//
// Created by Farias, Karine on 3/17/21.
//

#include "../include/ClientCommunicationManager.h"
#include "../../Utils/Types.h"
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <ctime>

int ClientCommunicationManager::establish_connection(std::string username, std::string server , std::string door ) {
    this->username = username;
    this->server = server;
    this->door = door;
    this->session_cookie = NO_COOKIE;

    char* bufferPayload = (char*) calloc(MAX_DATA_SIZE, sizeof(char));
    packet loginPacket = {
            0,0,0,0, bufferPayload
    };

    std::string listen_notification_port;
    listen_notifications(&listen_notification_port);

    buildLoginPacket(username, listen_notification_port, &loginPacket);
    sendPacket(&loginPacket);

    return 1;
}

void ClientCommunicationManager::sendPacket(struct __packet *packet) {
    int sockfd, n;
    struct sockaddr_in server_addr;
    struct hostent *server_host;
    struct in_addr addr;
    std::string buffer_out, buffer_in;

    inet_aton(server.c_str(), &addr);
    server_host = gethostbyaddr(&addr, sizeof(server), AF_INET);

    if (server_host == NULL) {
        printf("ERROR: no such host!!");
        exit(0);
    }

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        printf("ERROR opening socket\n");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(4000);
    server_addr.sin_addr = *((struct in_addr *)server_host->h_addr);
    bzero(&(server_addr.sin_zero), 8);

    if (connect(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
        printf("ERROR connecting\n");

    char* bufferPayload = (char*) calloc(MAX_DATA_SIZE, sizeof(char));
    packet->print("SENT");

    char* buffer;
    buffer = (char *) calloc(MAX_MAIL_SIZE, sizeof(char));
    buffer = packet->Serialize();

    // write
    n = write(sockfd, buffer, MAX_MAIL_SIZE);
    if (n < 0)
        printf("ERROR writing to socket\n");

    char buffer_response[MAX_MAIL_SIZE];
    bzero(buffer_response, MAX_MAIL_SIZE);

    n = read(sockfd, buffer_response, MAX_MAIL_SIZE);
    if (n < 0)
        printf("ERROR reading from socket\n");

    char received_packet_buffer[MAX_DATA_SIZE];
    struct __packet received_packet = {0, 0, 0, 0, received_packet_buffer };
    received_packet.Deserialize(buffer_response);
    received_packet.print("RECEIVED");

    if(this->session_cookie == NO_COOKIE)
        this->session_cookie = received_packet.cookie;

    close(sockfd);
}

int ClientCommunicationManager::follow(std::string followed)
{
    std::cout << "Hi! I am the communication Manager" << NEW_LINE;
    std::cout << "I am triggering a request to follow this profile:" << followed << NEW_LINE;
    return 0;
}

// Private Methods

void ClientCommunicationManager::buildLoginPacket(std::string username, std::string listen_notification_port, struct __packet *loginPacket) {
    uint16_t headerLength = 8; // cada uint16_t possui 2 bytes.
    uint16_t length = headerLength + MAX_DATA_SIZE;
    uint16_t timestamp = getTimestamp();

    loginPacket->type = CMD_LOGIN;
    loginPacket->seqn = 0;
    loginPacket->length = length;
    loginPacket->timestamp = timestamp;
    loginPacket->cookie = session_cookie;
    loginPacket->_payload = username + "\n" + listen_notification_port + "\n";
}

uint16_t ClientCommunicationManager::getTimestamp() {
    time_t ti;
    ti = time(NULL);
    struct tm tm_time;
    tm_time = *localtime(&ti);

    //const time_t create_time;
    uint16_t t, d;
    d = tm_time.tm_mday
        + (tm_time.tm_mon + 1) * 32
        + (tm_time.tm_year - (1980-1900)) * 512;

    // Print ddmmyy
//    printf("%02d%02d%02d\n",
//           (int) d%32, (int) (d/32)%16, (int) ((d/512)%128 + (1980-1900))%100);
    return d;
}

void ClientCommunicationManager::listen_notifications(std::string *listen_notification_port) {

    int notification_socket;
    socklen_t clilen;
    struct sockaddr_in notf_addr;
    std::string input;

    if ((notification_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        printf("ERROR opening socket\n");

    notf_addr.sin_family = AF_INET;
    notf_addr.sin_addr.s_addr = INADDR_ANY;

    bzero(&(notf_addr.sin_zero), 8);

    if (bind(notification_socket, (struct sockaddr *) &notf_addr, sizeof(notf_addr)) < 0)
        printf("[Notification Service] ERROR on binding\n");

    listen(notification_socket, 5);

    socklen_t len = sizeof(notf_addr);
    if (getsockname(notification_socket, (struct sockaddr *) &notf_addr, &len) == -1) {
        printf("[Notification Service] ERROR on getsockname\n");
    } else {
        *listen_notification_port = std::to_string(ntohs(notf_addr.sin_port));
        printf("[Notification Service] port number %d\n", ntohs(notf_addr.sin_port));
    }
    clilen = sizeof(struct sockaddr_in);

    printf("[Notification Service] Ready to receive\n");


    // cria um novo processo filho p/ ouvir notificações
    pid_t pid = fork();
    if (pid == 0) { // processo filho
        printf("started notification listening service\n");

        int n;

        //Todo: change loop to detect the end of execution, so it can return to the app;
        while (true) {

            int connection_socket;
            struct sockaddr_in cli_addr;

            if ((connection_socket = accept(notification_socket, (struct sockaddr *) &cli_addr, &clilen)) == -1) {
                printf("ERROR on accept");
                continue;
            }

            char buffer[MAX_MAIL_SIZE];
            bzero(buffer, MAX_MAIL_SIZE);
            n = read(connection_socket, buffer, MAX_MAIL_SIZE);
            if (n < 0)
                printf("[Notification Service] ERROR reading from socket");

            char received_packet_buffer[MAX_DATA_SIZE];
            struct __packet received_packet = {0, 0, 0, 0, received_packet_buffer };
            received_packet.Deserialize(buffer);
            received_packet.print("[Notification Service] RECEIVED");
        }

        close(notification_socket);
        exit(0);
    }

    return;
}
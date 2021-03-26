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
    packet loginPacket = {
            0,0,0,0, bufferPayload
    };

    buildLoginPacket(username, &loginPacket);
    loginPacket.print("SENT");

    char* buffer;
    buffer = (char *) calloc(MAX_MAIL_SIZE, sizeof(char));
    buffer = loginPacket.Serialize();

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

    close(sockfd);

    return 1;
}

int ClientCommunicationManager::follow(std::string followed)
{
    std::cout << "Hi! I am the communication Manager" << NEW_LINE;
    std::cout << "I am triggering a request to follow this profile:" << followed << NEW_LINE;
    return 0;
}

// Private Methods

void ClientCommunicationManager::buildLoginPacket(std::string username, struct __packet *loginPacket) {

    uint16_t headerLength = 8; // cada uint16_t possui 2 bytes.
    uint16_t length = headerLength + MAX_DATA_SIZE;
    uint16_t timestamp = getTimestamp();

    loginPacket->type = CMD_LOGIN;
    loginPacket->seqn = 0;
    loginPacket->length = length;
    loginPacket->timestamp = timestamp;
    loginPacket->_payload = username;
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
//
// Created by Farias, Karine on 3/17/21.
//

#include "../../Utils/Types.h"
#include "../include/ServerCommunicationManager.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <ctime>

#define PORT 4000

/**
 * Inicializa o connectionSocket, que fica aberto ouvindo mensagens de clientes enviadas na porta 4000.
 * Função desse socket é:
 * - Ouvir porta 4000
 * - Ao receber uma mensagem, criar uma nova thread com um clientSocket que irá tratá-la, e voltar a ouvir a porta.
 */

void ServerCommunicationManager::start( )
{
    int server_socket;
    socklen_t clilen;
    struct sockaddr_in serv_addr;
    std::string input;

    if((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        printf("ERROR opening socket");

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    bzero(&(serv_addr.sin_zero), 8);

    if (bind(server_socket, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        printf("ERROR on binding");

    listen(server_socket, 5);

    clilen = sizeof(struct sockaddr_in);

    printf("Server is ready to receive");

    //Todo: change loop to detect the end of execution, so it can return to the app;
    while (true) {

        int connection_socket;
        struct sockaddr_in cli_addr;

        if ((connection_socket = accept(server_socket, (struct sockaddr *) &cli_addr, &clilen)) == -1) {
            printf("ERROR on accept");
            continue;
        }

        makeCookie(&cli_addr);

        // cria um novo processo filho p/ cliente
        pid_t pid = fork();
        if (pid == 0) { // processo filho
            printf("started process child\n");
            start_client_thread(connection_socket, &cli_addr);
            exit(0);
        }
    }

    close(server_socket);
}

void ServerCommunicationManager::start_client_thread(int connection_socket, sockaddr_in *cli_addr) {
    int n;
    int response_code;
    char buffer[MAX_MAIL_SIZE];

    bzero(buffer, MAX_MAIL_SIZE);

    n = read(connection_socket, buffer, MAX_MAIL_SIZE);
    if (n < 0)
        printf("ERROR reading from socket");

    char received_packet_buffer[MAX_DATA_SIZE];
    struct __packet received_packet = {0, 0, 0, 0, NO_COOKIE, received_packet_buffer };
    received_packet.Deserialize(buffer);
    received_packet.print(std::string("RECEIVED"));

    std::string cookie;
    if(received_packet.cookie == NO_COOKIE)
        cookie = makeCookie(cli_addr);

    // CALLBACK METHOD TO HANDLE COMMAND EXECUTION
    response_code = taskManager.run_command(received_packet.type, std::string(received_packet._payload.c_str()), cookie); // enviar IP e porta.

    int type;
    std::string message;
    std::tuple<int, std::string> response_data = std::make_tuple(response_code, "");
    std::tie(type, message) = response_data;

    char response_packet_payload[MAX_DATA_SIZE];
    struct __packet response_packet = {0, 0, 0, 0, cookie, response_packet_payload };

    buildPacket(type, 0, message, &response_packet);

    char* response_buffer;
    response_buffer = (char *) calloc(MAX_MAIL_SIZE, sizeof(char));
    response_buffer = response_packet.Serialize();

    n = write(connection_socket, response_buffer, MAX_MAIL_SIZE);
    if (n < 0)
        printf("ERROR writing to socket");

    close(connection_socket);
    //printf("finished process child\n");
    //printf("closed connection_socket\n");

    // TODO: REMOVE LINES BELOW (TESTING NOTIFICATION)

//    sleep(5);
//
//    char* client_ip = inet_ntoa(cli_addr->sin_addr);
//    std::string ip_str(client_ip);
//
//    std::stringstream ss(received_packet._payload);
//    std::string to;
//
//
//    int i=0;
//    std::string client_port;
//    while(std::getline(ss,to,'\n')) {
//        std::cout << to << std::endl;
//        if(i == 1) {
//            client_port = to;
//            break;
//        }
//        i++;
//    }
//
//    sendNotification(client_ip, client_port, cookie, std::string("Tweet notification mock"));
}

void ServerCommunicationManager::buildPacket(uint16_t type, uint16_t seqn, std::string message, struct __packet *packet) {

    uint16_t headerLength = HEADER_LENGTH; // cada uint16_t possui 2 bytes.
    uint16_t length = headerLength + MAX_DATA_SIZE;
    uint16_t timestamp = getTimestamp();

    packet->type = type;
    packet->seqn = seqn;
    packet->length = length;
    packet->timestamp = timestamp;
    packet->_payload = message;
}

uint16_t ServerCommunicationManager::getTimestamp() {
    time_t ti;
    ti = time(NULL);
    struct tm tm_time;
    tm_time = *localtime(&ti);

    //const time_t create_time;
    //uint16_t t, d;
    uint16_t d;
    d = tm_time.tm_mday
        + (tm_time.tm_mon + 1) * 32
        + (tm_time.tm_year - (1980-1900)) * 512;

    // Print ddmmyy
//    printf("%02d%02d%02d\n",
//           (int) d%32, (int) (d/32)%16, (int) ((d/512)%128 + (1980-1900))%100);
    return d;
}

std::string ServerCommunicationManager::makeCookie(sockaddr_in *cli_addr) {

    char* ip_char_pointer = inet_ntoa(cli_addr->sin_addr);
    std::string ip_str(ip_char_pointer);

    char port_char_pointer[80];
    sprintf(port_char_pointer, "%u", htons(cli_addr->sin_port));
    std::string port_str(port_char_pointer);

    std::string cookie = ip_str + port_str;
    int cookie_prefix_length = cookie.length();

    if (cookie_prefix_length < COOKIE_LENGTH) {
        std::string random_suffix = random_string(COOKIE_LENGTH - cookie_prefix_length);
        cookie = ip_str + port_str + random_suffix;
    } else {
        int extra_chars = cookie_prefix_length - COOKIE_LENGTH;
        cookie.erase(cookie.length()-extra_chars);
    }

    return cookie;
}

std::string ServerCommunicationManager::random_string( size_t length )
{
    auto randchar = []() -> char
    {
        const char charset[] =
                "0123456789"
                "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                "abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[ rand() % max_index ];
    };
    std::string str(length,0);
    std::generate_n( str.begin(), length, randchar );
    return str;
}

void ServerCommunicationManager::sendNotification(std::string session_id, std::string message) {
    client_session session = this->client_sessions[session_id];
    this->sendNotification(session.ip, session.notification_port, session_id, message);
}

void ServerCommunicationManager::sendNotification(std::string receiver_ip, std::string receiver_port, std::string session_id, std::string message) {

    int sockfd, n;
    struct sockaddr_in receiver_addr;
    struct hostent *receiver_host;
    struct in_addr addr;
    std::string buffer_out;

    inet_aton(receiver_ip.c_str(), &addr);
    receiver_host = gethostbyaddr(&addr, sizeof(receiver_ip), AF_INET);

    if (receiver_host == NULL) {
        printf("ERROR: no such client found!!");
        exit(0);
    }

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        printf("ERROR opening socket\n");

    printf("[Send Notification] receiver port: %s\n", receiver_port.c_str());

    receiver_addr.sin_family = AF_INET;
    receiver_addr.sin_port = htons(std::stoi(receiver_port.c_str()));
    receiver_addr.sin_addr = *((struct in_addr *)receiver_host->h_addr);
    bzero(&(receiver_addr.sin_zero), 8);

    if (connect(sockfd, (struct sockaddr *) &receiver_addr, sizeof(receiver_addr)) < 0)
        printf("ERROR connecting\n");

    char* bufferPayload = (char*) calloc(MAX_DATA_SIZE, sizeof(char));
    packet packet_sent = {0,0,0,0, session_id, bufferPayload };

    buildPacket(NOTIFICATION_TWEET, 0, message, &packet_sent);

    packet_sent.print(std::string("SENT"));

    char* buffer;
    buffer = (char *) calloc(MAX_MAIL_SIZE, sizeof(char));
    buffer = packet_sent.Serialize();

    // write
    n = write(sockfd, buffer, MAX_MAIL_SIZE);
    if (n < 0)
        printf("ERROR writing to socket\n");

    close(sockfd);
}
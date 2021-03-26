//
// Created by Farias, Karine on 3/17/21.
//

#include "../../Utils/Types.h"
#include "../include/ServerCommunicationManager.h"
#include "../include/TaskManager.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

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

        // cria um novo processo filho p/ cliente
        pid_t pid = fork();
        if (pid == 0) { // processo filho
            printf("started process child");
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
    TaskManager taskManager;
    std::string session_id = std::to_string( cli_addr->sin_port );

    bzero(buffer, MAX_MAIL_SIZE);

    n = read(connection_socket, buffer, MAX_MAIL_SIZE);
    if (n < 0)
        printf("ERROR reading from socket");

    char received_packet_buffer[MAX_DATA_SIZE];
    struct __packet received_packet = {0, 0, 0, 0, received_packet_buffer };
    received_packet.Deserialize(buffer);
    received_packet.print("RECEIVED");

    std::cout << "Hi, I am here" << NEW_LINE;

    // CALLBACK METHOD TO HANDLE COMMAND EXECUTION
    response_code = taskManager.run_command(received_packet.type, received_packet._payload, session_id);

    std::cout << "Hi, have executed that shitty method" << NEW_LINE;

    int type;
    std::string message;
    std::tuple<int, std::string> response_data = std::make_tuple(response_code, "Response message");
    std::tie(type, message) = response_data;

    char response_packet_payload[MAX_DATA_SIZE];
    struct __packet response_packet = {0, 0, 0, 0, response_packet_payload };
    buildPacket(&response_packet, type, 0, message);

    char* response_buffer;
    response_buffer = (char *) calloc(MAX_MAIL_SIZE, sizeof(char));
    response_buffer = response_packet.Serialize();

    n = write(connection_socket, response_buffer, MAX_MAIL_SIZE);
    if (n < 0)
        printf("ERROR writing to socket");

    close(connection_socket);
    printf("finished process child\n");
    printf("closed connection_socket\n");
}

//uint16_t type;          //PACKAGE TYPE (Eg. data/command)
//uint16_t seqn;          //Sequence number
//uint16_t length;        //Payload size
//uint16_t timestamp;     //Data timestamp
//std::string _payload;   //Message data


void ServerCommunicationManager::buildPacket(struct __packet *packet, uint16_t type, uint16_t seqn, std::string message) {

    uint16_t headerLength = 8; // cada uint16_t possui 2 bytes.
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
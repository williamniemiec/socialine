//
// Created by Farias, Karine on 3/17/21.
//

#ifndef SOCIALINEV2_SERVERCOMMUNICATIONMANAGER_H
#define SOCIALINEV2_SERVERCOMMUNICATIONMANAGER_H

#include <stdlib.h>
#include <string.h>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>

#include "../include/TaskManager.h"

typedef struct __client_session {
    std::string session_id;
    std::string ip;
    std::string notification_port;

} client_session;


class ServerCommunicationManager {
    TaskManager taskManager;
public:
    void start( );
    void sendNotification(std::string session_id, std::string message);
private:
    std::unordered_map<std::string, client_session> client_sessions;

    void sendNotification(std::string receiver_ip, std::string receiver_port, std::string session_id, std::string message);
    void buildPacket(uint16_t type, uint16_t seqn, std::string message, struct __packet *loginPacket);
    void start_client_thread(int connection_socket, sockaddr_in *cli_addr);
    std::string makeCookie(sockaddr_in *cli_addr);
    uint16_t getTimestamp();
    std::string random_string(size_t length);
};


#endif //SOCIALINEV2_SERVERCOMMUNICATIONMANAGER_H
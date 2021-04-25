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
#include "../../Utils/Types.h"
#include "../../Utils/StringUtils.h"
#include "../../Utils/Logger.h"

typedef struct __client_session {
    std::string session_id;
    std::string ip;
    std::string notification_port;

} client_session;


class ServerCommunicationManager {
    static std::unordered_map<std::string, client_session> client_sessions;
public:
    static void start( );
    static void sendNotification(std::string session_id, notification current_notification);
private:

    static void sendNotification(std::string receiver_ip, std::string receiver_port, std::string session_id, std::string message);
    static void buildPacket(uint16_t type, uint16_t seqn, std::string message, struct __packet *loginPacket);
    static void start_client_thread(int connection_socket, sockaddr_in *cli_addr);
    static std::string makeCookie(sockaddr_in *cli_addr);
    static uint16_t getTimestamp();
    static std::string random_string(size_t length);
};


#endif //SOCIALINEV2_SERVERCOMMUNICATIONMANAGER_H
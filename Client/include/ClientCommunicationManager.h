//
// Created by Farias, Karine on 3/17/21.
//

#ifndef SOCIALINEV2_CLIENTCOMMUNICATIONMANAGER_H
#define SOCIALINEV2_CLIENTCOMMUNICATIONMANAGER_H

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

class ClientCommunicationManager {
    std::string username, server, door, session_cookie;
public:
    int establish_connection(std::string username, std::string server , std::string door );
    int follow(std::string followed);

private:
    void listen_notifications(std::string *listen_notification_port);
    void buildLoginPacket(std::string username, std::string listen_notification_port, struct __packet *loginPacket);
    void sendPacket(struct __packet *packet);
    uint16_t getTimestamp();
};


#endif //SOCIALINEV2_CLIENTCOMMUNICATIONMANAGER_H
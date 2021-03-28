//
// Created by Farias, Karine on 3/17/21.
//

#ifndef SOCIALINEV2_CLIENTCOMMUNICATIONMANAGER_H
#define SOCIALINEV2_CLIENTCOMMUNICATIONMANAGER_H

#include <iostream>

class ClientCommunicationManager {
    std::string username, server, door, session_cookie;
public:
    int establish_connection(std::string username, std::string server , std::string door );
    int follow(std::string followed);

private:
    void buildLoginPacket(std::string username, struct __packet *loginPacket);
    uint16_t getTimestamp();
};


#endif //SOCIALINEV2_CLIENTCOMMUNICATIONMANAGER_H
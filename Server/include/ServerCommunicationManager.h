//
// Created by Farias, Karine on 3/17/21.
//

#ifndef SOCIALINEV2_SERVERCOMMUNICATIONMANAGER_H
#define SOCIALINEV2_SERVERCOMMUNICATIONMANAGER_H

#include <stdlib.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>

class ServerCommunicationManager {
public:
    void start( );
private:
    void buildPacket(struct __packet *loginPacket, uint16_t type, uint16_t seqn, std::string message);
    void start_client_thread(int connection_socket, sockaddr_in *cli_addr);
    uint16_t getTimestamp();
};


#endif //SOCIALINEV2_SERVERCOMMUNICATIONMANAGER_H
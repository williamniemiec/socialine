//
// Created by Farias, Karine on 3/17/21.
//

#ifndef SOCIALINEV2_CLIENTCOMMUNICATIONMANAGER_H
#define SOCIALINEV2_CLIENTCOMMUNICATIONMANAGER_H

#include <iostream>

class ClientCommunicationManager {
    public:
        int establish_connection(std::string username, std::string server , std::string door );
        int follow(std::string followed);
};


#endif //SOCIALINEV2_CLIENTCOMMUNICATIONMANAGER_H

//
// Created by Farias, Karine on 3/17/21.
//

#include "../include/ClientCommunicationManager.h"
#include "../../Utils/Types.h"
#include <iostream>

int ClientCommunicationManager::establish_connection(std::string username, std::string server , std::string door ) {

    std::cout << "Hi! I am the communication Manager" << NEW_LINE;
    std::cout << "I am trying to establish connection" << NEW_LINE;
    return 0;
}

int ClientCommunicationManager::follow(std::string followed)
{
    std::cout << "Hi! I am the communication Manager" << NEW_LINE;
    std::cout << "I am triggering a request to follow this profile:" << followed << NEW_LINE;
    return 0;
}
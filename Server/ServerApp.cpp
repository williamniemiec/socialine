//
// Created by Farias, Karine on 3/17/21.
//


#include "include/ServerCommunicationManager.h"
#include "../Utils/Types.h"

#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

int main(int argc, char* argv[])
{
    ServerCommunicationManager server;

    server.start();

    //ToDo: call function to persist server data

    cout << "I am finishing the session!! " << NEW_LINE;

    return(0);
}
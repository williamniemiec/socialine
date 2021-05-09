#ifndef SOCIALINEV2_FRONTEND_H
#define SOCIALINEV2_FRONTEND_H

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

#include <pthread.h>

#include "../../../Utils/StringUtils.h"
#include "../../../Utils/Types.h"
#include "../../../Utils/Logger.h"

class Frontend {
//    static std::string username, server, door, session_cookie;
//    static int notification_socket;
//    static socklen_t clilen;
//    static models::manager::ClientNotificationManager* notificationManager;

public:
    static std::string primaryServerIP;
    static uint16_t primaryServerPort;
    static std::string broadcastIP;

    static void discoverPrimaryServer();
    static void updatePrimaryServer(std::string newIP, int newPort);

private:

};


#endif

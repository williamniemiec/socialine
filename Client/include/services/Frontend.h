#ifndef SOCIALINEV2_FRONTEND_H
#define SOCIALINEV2_FRONTEND_H


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

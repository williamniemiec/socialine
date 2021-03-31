//
// Created by Farias, Karine on 3/17/21.
//

#ifndef SOCIALINEV2_SERVERNOTIFICATIONMANAGER_H
#define SOCIALINEV2_SERVERNOTIFICATIONMANAGER_H

#include "../include/ProfileSessionManager.h"
#include "../../Utils/Types.h"

class ServerNotificationManager {

    static std::unordered_map<std::string, std::vector<notification>> pending_notifications;
    static sem_t notifications_semaphore;


public:
    ServerNotificationManager( )
    {
        sem_init(&notifications_semaphore, 1, 1);
    }

    //Method used to read all pending notifications for a given username
    //returns:
    //vector containing the session ids of the receivers
    std::vector<notification> read_notifications( std::string username );

    //Method to handle new tweets
    //returns:
    //vector of session ids containing the session ids of the receivers
    std::unordered_map<std::string, notification> tweet( std::string username, std::string message, std::string timestamp );

private:
    void add_pending_notification( std::string followed, notification current_notification );

};


#endif //SOCIALINEV2_SERVERNOTIFICATIONMANAGER_H
//
// Created by Farias, Karine on 3/17/21.
//

#ifndef SOCIALINEV2_SERVERNOTIFICATIONMANAGER_H
#define SOCIALINEV2_SERVERNOTIFICATIONMANAGER_H

#include "../include/ProfileSessionManager.h"
#include "../../Utils/Types.h"
#include "../../Utils/Logger.h"
#include "../../Utils/IObservable.hpp"
#include "../../Utils/IObserver.hpp"
#include <semaphore.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/sem.h>

class ServerNotificationManager : public IObservable
{
    std::list<IObserver*> observers;
    static std::unordered_map<std::string, std::vector<notification>> pending_notifications;
    static sem_t notifications_semaphore;


public:
    ServerNotificationManager( )
    {
        observers = std::list<IObserver*>();
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

    static std::unordered_map<std::string, std::vector<notification>> get_pending_notifications();
    virtual void attach(IObserver* observer);
    virtual void detatch(IObserver* observer);
    virtual void notify_observers();
    static void add_pending_notification( std::string followed, notification current_notification );
    static void remove_pending_notification(std::string username);
};


#endif //SOCIALINEV2_SERVERNOTIFICATIONMANAGER_H
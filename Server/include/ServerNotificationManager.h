#ifndef SOCIALINEV2_SERVERNOTIFICATIONMANAGER_H
#define SOCIALINEV2_SERVERNOTIFICATIONMANAGER_H

#include "../include/ReplicManager.hpp"
#include "../../Utils/Types.h"
#include "../../Utils/Logger.h"
#include "../../Utils/IObservable.hpp"
#include "../../Utils/IObserver.hpp"
#include <semaphore.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/sem.h>

/// <summary>
///     Responsible for managing client notifications.
/// <summary>
class ServerNotificationManager : public IObservable
{
//-------------------------------------------------------------------------
//		Attributes
//-------------------------------------------------------------------------
private:
    static std::unordered_map<std::string, std::vector<notification>> pending_notifications;
    static sem_t notifications_semaphore;
    std::list<IObserver*> observers;
    std::string arg0;
    std::string arg1;
    std::string arg2;
    ReplicManager* rm;


//-------------------------------------------------------------------------
//		Constructor
//-------------------------------------------------------------------------
public:
    ServerNotificationManager( )
    {
        rm = ReplicManager::get_instance();
        observers = std::list<IObserver*>();
        sem_init(&notifications_semaphore, 1, 1);
        arg0 = "";
        arg1 = "";
        arg2 = "";
        observers.push_back(rm);
    }


//-------------------------------------------------------------------------
//		Methods
//-------------------------------------------------------------------------
public:
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

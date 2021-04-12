#pragma once
#include <list>
#include "../../../../Utils/Types.h"
#include "../Notification.hpp"
#include "../IObservable.hpp"
#include "../IObserver.hpp"

namespace models::manager
{
    class ClientNotificationManager : public models::IObservable
    {
    //-------------------------------------------------------------------------
    //      Attributes
    //-------------------------------------------------------------------------
    private:
        static std::list<models::IObserver*> observers;
        static std::list<std::string>* notifications;
        static ClientNotificationManager* instance;


    //-------------------------------------------------------------------------
    //      Constructor
    //-------------------------------------------------------------------------
    public:
        ClientNotificationManager();
        

    //-------------------------------------------------------------------------
    //      Methods
    //-------------------------------------------------------------------------
    public:
        void attach(models::IObserver* observer);
        void detatch(models::IObserver* observer);
        void notify_observers();
        //void fetch_notifications();
        void receive_notification(std::string username, uint32_t timestamp, std::string message);
        static ClientNotificationManager* get_instance();
    };
}

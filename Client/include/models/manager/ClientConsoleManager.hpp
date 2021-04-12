#pragma once
#include "../IObservable.hpp"
#include "../IObserver.hpp"

namespace models::manager
{
    class ClientConsoleManager : public models::IObservable
    {
    //-------------------------------------------------------------------------
    //      Attributes
    //-------------------------------------------------------------------------
    private:
        std::list<models::IObserver*> observers;
        std::list<std::string>* notifications;
        std::string message;


    //-------------------------------------------------------------------------
    //      Methods
    //-------------------------------------------------------------------------
    public:
        void attach(models::IObserver* observer);
        void detatch(models::IObserver* observer);
        void notify_observers();
        void send(std::string message);
        void welcome_message(std::string username);
        std::string generate_date_time();
    };
}

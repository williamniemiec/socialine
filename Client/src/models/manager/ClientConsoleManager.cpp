#include <list>
#include <ctime>
#include <sstream>
#include "../../../include/models/manager/ClientConsoleManager.hpp"

using namespace models::manager;

//-------------------------------------------------------------------------
//      Methods
//-------------------------------------------------------------------------
void ClientConsoleManager::attach(models::IObserver* observer)
{
    observers.push_back(observer);
}

void ClientConsoleManager::detatch(models::IObserver* observer)
{
    observers.remove(observer);
}

void ClientConsoleManager::notify_observers()
{
    std::list<std::string> data;
    data.push_back(message);

    for (models::IObserver* observer : observers)
    {
        observer->update(this, data);
    }
}

void ClientConsoleManager::send(std::string message)
{
    this->message = "{ " + generateDateTime() + " } " + message;

    notify_observers();
}

std::string ClientConsoleManager::generateDateTime()
{
    time_t t = time(0);
    struct tm timeinfo = *localtime(&t);

    std::stringstream date;

    if ((timeinfo.tm_mon+1) < 10)
        date << "0";

    date << timeinfo.tm_mon+1;
    date << "/";

    if ((timeinfo.tm_mday+1) < 10)
        date << "0";

    date << timeinfo.tm_mday+1;
    date << "/";
    date << timeinfo.tm_year+1900;
    date << " ";

    if ((timeinfo.tm_hour) < 10)
        date << "0";

    date << timeinfo.tm_hour;
    date << ":";

    if ((timeinfo.tm_min) < 10)
        date << "0";

    date << timeinfo.tm_min;
    date << ":";

    if ((timeinfo.tm_sec) < 10)
        date << "0";

    date << timeinfo.tm_sec;

    return date.str();
}

void ClientConsoleManager::welcomeMessage(std::string username)
{
    send("Welcome back " + username);
}

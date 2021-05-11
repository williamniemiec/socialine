//
// Created by Farias, Karine on 3/17/21.
//

#ifndef SOCIALINEV2_SERVERCOMMUNICATIONMANAGER_H
#define SOCIALINEV2_SERVERCOMMUNICATIONMANAGER_H

#include <stdlib.h>
#include <string.h>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>

#include "../include/TaskManager.h"
#include "../include/ReplicManager.hpp"
#include "../../Utils/IObservable.hpp"
#include "../../Utils/IObserver.hpp"
#include "../../Utils/Types.h"
#include "../../Utils/StringUtils.h"
#include "../../Utils/Logger.h"

class ServerCommunicationManager : public IObserver, public IObservable
{
    std::list<IObserver*> observers;
    static std::unordered_map<std::string, client_session> client_sessions;
    static ReplicManager* replic_manager;
public:
    static bool isPrimaryServer;
    ServerCommunicationManager();
    virtual void update(IObservable* observable, std::list<std::string> data);
    virtual void attach(IObserver* observer);
    virtual void detatch(IObserver* observer);
    virtual void notify_observers();
    void start( );
    static void sendNotification(std::string session_id, notification current_notification);
    static void listenForBroadcast();
    static void* updateClientsWithNewPrimaryServer(void* arg);
    std::unordered_map<std::string, client_session> get_sessions();

private:
    static void initialize_replic_manager();
    static void sendNotification(std::string receiver_ip, std::string receiver_port, std::string session_id, std::string message);
    static void buildPacket(uint16_t type, uint16_t seqn, std::string message, struct __packet *loginPacket);
    void start_client_thread(int connection_socket, sockaddr_in *cli_addr);
    static std::string makeCookie(sockaddr_in *cli_addr);
    static uint16_t getTimestamp();
    static std::string random_string(size_t length);
    static void* threadListenForBroadcast(void* arg);
    static std::string getIP();

};


#endif //SOCIALINEV2_SERVERCOMMUNICATIONMANAGER_H
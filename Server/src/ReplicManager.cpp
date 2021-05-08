/*
    ESSA CLASSE SERÁ REFATORADA MAIS TARDE! 
    
    FAVOR NÃO REPARAR NA BAGUNÇA :D
*/

// TODO: refactor class and methods

#include <unistd.h>
#include <iostream>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <thread>
#include <ctime>
#include "../include/ReplicManager.hpp"
#include "../../Utils/Scheduler.hpp"
#include "../../Utils/Logger.h"

#define MSG_HEARTBEAT 0
#define MSG_NEW_SESSION 1
#define MSG_NEW_PENDING_NOTIFICATION 2
#define MSG_CLOSE_SESSION 3
#define MSG_FOLLOW 4
#define MSG_NEW_AVAILABLE_PORT 6
#define MSG_LIST_BACKUP 7
#define MSG_PRIMARY_ADDR 8
#define TIMEOUT_SERVER_MS 6000
#define HEARTBEAT_MS 3000
#define MAX_WAITING_BACKUPS 5
#define PORT_MULTICAST 4001
#define PORT_HEARTBEAT 4002

using namespace socialine::util::task;
using namespace socialine::utils;

//-------------------------------------------------------------------------
//		Attributes
//-------------------------------------------------------------------------
int ReplicManager::g_availablePort = 6001;
int ReplicManager::g_process_id = 0;
std::map<int, sockaddr_in> *ReplicManager::rm = new std::map<int, sockaddr_in>();
socklen_t ReplicManager::clilen = sizeof(struct sockaddr_in);
bool ReplicManager::g_primary_server_online = true;
int ReplicManager::server_socket;
int ReplicManager::connection_socket;
struct sockaddr_in ReplicManager::cli_addr;
int ReplicManager::readBytesFromSocket;
char ReplicManager::buffer_response[MAX_MAIL_SIZE];
std::string ReplicManager::primaryIp;
std::string ReplicManager::multicastIp = "226.1.1.1";


//-------------------------------------------------------------------------
//		Methods
//-------------------------------------------------------------------------
void ReplicManager::run()
{
    bool hasPrimaryActive = try_receive_multicast_signal();

    if (hasPrimaryActive)
    {
        std::cout << "THERE IS AN ACTIVE PRIMARY!" << std::endl;
        init_server_as_backup();
    }
    else
    {
        std::cout << "THERE IS NO ACTIVE PRIMARY!" << std::endl;
        init_server_as_primary();
    }
}


//
//      PRIMARY OR BACKUP SERVER
//

bool ReplicManager::try_receive_multicast_signal()
{
    struct sockaddr_in localSock;
    struct ip_mreq group;

    /* Create a datagram socket on which to receive. */
    connection_socket = socket(AF_INET, SOCK_DGRAM, 0);

    if (connection_socket < 0)
    {
        perror("Opening datagram socket error");
        exit(1);
    }
    else
        printf("Opening datagram socket....OK.\n");

    /* Enable SO_REUSEADDR to allow multiple instances of this */
    /* application to receive copies of the multicast datagrams. */
    {
        int reuse = 1;
        if (setsockopt(connection_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse)) < 0)
        {
            perror("Setting SO_REUSEADDR error");
            close(connection_socket);
            exit(1);
        }
        else
            printf("Setting SO_REUSEADDR...OK.\n");
    }

    /* Bind to the proper port number with the IP address */
    /* specified as INADDR_ANY. */
    memset((char *)&localSock, 0, sizeof(localSock));
    localSock.sin_family = AF_INET;
    localSock.sin_port = htons(PORT_HEARTBEAT);
    localSock.sin_addr.s_addr = INADDR_ANY;
    if (bind(connection_socket, (struct sockaddr *)&localSock, sizeof(localSock)))
    {
        perror("Binding datagram socket error");
        close(connection_socket);
        exit(1);
    }
    else
        printf("Binding datagram socket...OK.\n");
    
    /* Join the multicast group 226.1.1.1 on the local 203.106.93.94 */
    /* interface. Note that this IP_ADD_MEMBERSHIP option must be */
    /* called for each local interface over which the multicast */
    /* datagrams are to be received. */
    group.imr_multiaddr.s_addr = inet_addr(multicastIp.c_str());
    group.imr_interface.s_addr = inet_addr(get_local_ip().c_str());
    if (setsockopt(connection_socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&group, sizeof(group)) < 0)
    {
        perror("Adding multicast group error");
        close(connection_socket);
        exit(1);
    }
    else
        printf("Adding multicast group...OK.\n");
    /* Read from the socket. */
    
    printf("Reading multicast group...\n");
    
    Scheduler::set_timeout_to_routine([]()
    {
        readBytesFromSocket = read(connection_socket, buffer_response, MAX_MAIL_SIZE);
    }, 5000);

    if (readBytesFromSocket <= 0)
    {
        perror("Reading datagram message error");
        close(connection_socket);
        return false;
    }
    
    printf("Reading datagram message...OK.\n");
    printf("The message from multicast server is: \"%s\"\n", buffer_response);
    primaryIp = std::string(buffer_response);
    close(connection_socket);
    
    return true;
}

std::string ReplicManager::get_local_ip()
{
    int sock = socket(PF_INET, SOCK_DGRAM, 0);
    sockaddr_in loopback;

    if (sock == -1)
    {
        std::cerr << "Could not socket\n";
        exit(-1);
    }

    std::memset(&loopback, 0, sizeof(loopback));
    loopback.sin_family = AF_INET;
    loopback.sin_addr.s_addr = 1337; // can be any IP address
    loopback.sin_port = htons(9);    // using debug port

    if (connect(sock, reinterpret_cast<sockaddr *>(&loopback), sizeof(loopback)) == -1)
    {
        close(sock);
        std::cerr << "Could not connect\n";
        exit(-1);
    }

    socklen_t addrlen = sizeof(loopback);
    if (getsockname(sock, reinterpret_cast<sockaddr *>(&loopback), &addrlen) == -1)
    {
        close(sock);
        std::cerr << "Could not getsockname\n";
        exit(-1);
    }

    close(sock);

    char buf[INET_ADDRSTRLEN];
    if (inet_ntop(AF_INET, &loopback.sin_addr, buf, INET_ADDRSTRLEN) == 0x0)
    {
        std::cerr << "Could not inet_ntop\n";
        //return 1;
        exit(-1);
    }

    return std::string(buf);
}

bool ReplicManager::is_primary_active()
{
    int sockfd, n;
    struct sockaddr_in server_addr;
    struct hostent *server_host;
    struct in_addr addr;

    inet_aton(primaryIp.c_str(), &addr);
    server_host = gethostbyaddr(&addr, sizeof(primaryIp), AF_INET);

    if (server_host == NULL)
    {
        return false;
    }

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        return false;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT_HEARTBEAT);
    server_addr.sin_addr = *((struct in_addr *)server_host->h_addr);
    bzero(&(server_addr.sin_zero), 8);

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
        return false;

    close(sockfd);

    return true;
}



//
//      PRIMARY SERVER
//

void ReplicManager::init_server_as_primary()
{
    std::cout << "BECOMING PRIMARY..." << std::endl;
    primaryIp = get_local_ip();

    std::thread thread_heartbeat_receiver(heartbeat_receiver);
    thread_heartbeat_receiver.detach();

    std::thread thread_new_backup(service_new_backup);
    thread_new_backup.detach();

    Scheduler::set_interval([]() 
    {
        multicast_signal();
    }, 3000);

    std::cout << "DONE" << std::endl;

    std::thread thread_heartbeat_sender(heartbeat_sender);
    thread_heartbeat_sender.join();
}

void ReplicManager::multicast_signal()
{
    struct in_addr localInterface;
    struct sockaddr_in groupSock;

    strcpy(buffer_response, get_local_ip().c_str());


    connection_socket = socket(AF_INET, SOCK_DGRAM, 0);

    if (connection_socket < 0)
    {
        perror("Opening datagram socket error");
        exit(1);
    }
    else
        printf("Opening the datagram socket...OK.\n");

    /* Initialize the group sockaddr structure with a */
    /* group address of 225.1.1.1 and port 5555. */
    memset((char *)&groupSock, 0, sizeof(groupSock));
    groupSock.sin_family = AF_INET;
    groupSock.sin_addr.s_addr = inet_addr(multicastIp.c_str());
    groupSock.sin_port = htons(PORT_HEARTBEAT);

    /* Set local interface for outbound multicast datagrams. */
    /* The IP address specified must be associated with a local, */
    /* multicast capable interface. */
    localInterface.s_addr = inet_addr(get_local_ip().c_str());
    if (setsockopt(connection_socket, IPPROTO_IP, IP_MULTICAST_IF, (char *)&localInterface, sizeof(localInterface)) < 0)
    {
        perror("Setting local interface error");
        fprintf(stderr, "ERROR: %s\n", strerror(errno));
        exit(1);
    }
    else
        printf("Setting the local interface...OK\n");
    
    /* Send a message to the multicast group specified by the*/
    /* groupSock sockaddr structure. */
    if (sendto(connection_socket, buffer_response, MAX_MAIL_SIZE, 0, (struct sockaddr *)&groupSock, sizeof(groupSock)) < 0)
    {
        perror("Sending datagram message error");
    }
    else
        printf("Sending datagram message...OK\n");
}

void ReplicManager::heartbeat_sender()
{
    std::time_t start = std::time(nullptr);

    while (true)
    {
        if (difftime(std::time(nullptr), start) * 1000 >= (double)HEARTBEAT_MS)
        {
            // SEND HEARTBEAT TO ALL BACKUPS EACH 5 SECONDS
            for (auto it = rm->begin(); it != rm->end(); it++)
            {
                int sockfd, n;
                struct sockaddr_in backup_server_addr;

                backup_server_addr.sin_family = AF_INET;
                backup_server_addr.sin_port = htons(it->first);
                backup_server_addr.sin_addr = (it->second).sin_addr;
                bzero(&(backup_server_addr.sin_zero), 8);

                if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
                {
                    Logger.write_error("ERROR: Opening socket");
                    continue;
                }

                if (connect(sockfd, (struct sockaddr *)&backup_server_addr, sizeof(backup_server_addr)) < 0)
                {
                    //fprintf(stderr, "socket() failed: %s\n", strerror(errno));
                    std::cout << "BACKUP SERVER OFFLINE - REMOVED FROM BACKUP LIST" << std::endl;
                    rm->erase(it->first);
                    break; // Avoids segmentation fault
                }

                std::cout << "PRIMARY IS SENDING HEARTBEAT TO BACKUP AT PORT " << it->first << std::endl;
                char *message = new char[MAX_MAIL_SIZE];

                message[0] = MSG_HEARTBEAT;
                n = write(sockfd, message, MAX_MAIL_SIZE);
                if (n < 0)
                    Logger.write_error("Failed to write to socket");
            }

            start = std::time(nullptr);
        }
    }
}

void ReplicManager::heartbeat_receiver()
{
    int server_socket;
    socklen_t clilen;
    struct sockaddr_in serv_addr;
    struct in_addr addr;
    hostent *server_host;
    std::string input;
    
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        std::cout << "erro socket";
        exit(-1);
    }

    inet_aton(primaryIp.c_str(), &addr);
    server_host = gethostbyaddr(&addr, sizeof(primaryIp), AF_INET);

    if (server_host == NULL)
    {
        Logger.write_error("No such host!");
        exit(-1);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT_HEARTBEAT);
    serv_addr.sin_addr = *((struct in_addr *)server_host->h_addr);

    bzero(&(serv_addr.sin_zero), 8);

    if (bind(server_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        int option = 1;
        if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) < 0)
        {
            std::cout << "erro bind - backup server";
            fprintf(stderr, "socket() failed: %s\n", strerror(errno));
            exit(-1);
        }
    }

    listen(server_socket, 3);

    clilen = sizeof(struct sockaddr_in);

    while (true)
    {
        int connection_socket;
        struct sockaddr_in cli_addr;

        if ((connection_socket = accept(server_socket, (struct sockaddr *)&cli_addr, &clilen)) == -1)
        {
            close(connection_socket);
        }

        close(connection_socket);
    }

    close(server_socket);
}

void ReplicManager::notify_list_backups(std::map<int, sockaddr_in> *backups)
{
    for (auto it = backups->begin(); it != backups->end(); it++)
    {
        int sockfd, n;
        struct sockaddr_in backup_server_addr;

        backup_server_addr.sin_family = AF_INET;
        backup_server_addr.sin_port = htons(it->first);
        backup_server_addr.sin_addr = (it->second).sin_addr;
        bzero(&(backup_server_addr.sin_zero), 8);

        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        {
            Logger.write_error("ERROR: Opening socket");
            continue;
        }

        if (connect(sockfd, (struct sockaddr *)&backup_server_addr, sizeof(backup_server_addr)) < 0)
        {
            std::cout << "BACKUP SERVER OFFLINE - IT WILL BE SKIPPED" << std::endl;
            continue;
        }

        std::cout << "PRIMARY IS SENDING BACKUP LIST TO BACKUP AT PORT " << it->first << std::endl;
        char *message = new char[MAX_MAIL_SIZE];

        message[0] = MSG_LIST_BACKUP;

        uint8_t totalBackups = backups->size();
        int i = 0;

        message[1] = totalBackups;

        for (auto it = backups->begin(); it != backups->end(); it++)
        {
            uint32_t normalizedPort = htonl(it->first);
            sockaddr_in server = it->second;

            message[2 + i * sizeof(sockaddr_in) + 0] = normalizedPort >> 24; // MSB
            message[2 + i * sizeof(sockaddr_in) + 1] = normalizedPort >> 16;
            message[2 + i * sizeof(sockaddr_in) + 2] = normalizedPort >> 8;
            message[2 + i * sizeof(sockaddr_in) + 3] = normalizedPort; // LSB

            memcpy(&message[2 + i * sizeof(sockaddr_in) + 4], &server, sizeof(sockaddr_in));

            i++;
        }

        n = write(sockfd, message, MAX_MAIL_SIZE);

        if (n < 0)
            Logger.write_error("Failed to write to socket");
    }
}

void ReplicManager::notify_close_session(client_session session)
{
    for (auto it = rm->begin(); it != rm->end(); it++)
    {
        int sockfd, n;
        struct sockaddr_in backup_server_addr;

        backup_server_addr.sin_family = AF_INET;
        backup_server_addr.sin_port = htons(it->first);
        backup_server_addr.sin_addr = (it->second).sin_addr;
        bzero(&(backup_server_addr.sin_zero), 8);

        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        {
            Logger.write_error("ERROR: Opening socket");
            continue;
        }

        if (connect(sockfd, (struct sockaddr *)&backup_server_addr, sizeof(backup_server_addr)) < 0)
        {
            std::cout << "BACKUP SERVER OFFLINE - IT WILL BE SKIPPED" << std::endl;
            continue;
        }

        std::cout << "PRIMARY IS SENDING CLOSE SESSION TO BACKUP AT PORT " << it->first << std::endl;
        char *message = new char[MAX_MAIL_SIZE];

        message[0] = MSG_CLOSE_SESSION;

        // COOKIE
        memcpy(&message[1], session.session_id.c_str(), COOKIE_LENGTH);

        // IP
        memcpy(&message[1 + COOKIE_LENGTH], session.ip.c_str(), 16);

        // NOTIFICATION PORT
        memcpy(&message[1 + COOKIE_LENGTH + 16], session.notification_port.c_str(), 6);

        n = write(sockfd, message, MAX_MAIL_SIZE);

        if (n < 0)
            Logger.write_error("Failed to write to socket");
    }
}

void ReplicManager::notify_follow(std::string follower, std::string followed)
{
    for (auto it = rm->begin(); it != rm->end(); it++)
    {
        int sockfd, n;
        struct sockaddr_in backup_server_addr;

        backup_server_addr.sin_family = AF_INET;
        backup_server_addr.sin_port = htons(it->first);
        backup_server_addr.sin_addr = (it->second).sin_addr;
        bzero(&(backup_server_addr.sin_zero), 8);

        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        {
            Logger.write_error("ERROR: Opening socket");
            continue;
        }

        if (connect(sockfd, (struct sockaddr *)&backup_server_addr, sizeof(backup_server_addr)) < 0)
        {
            std::cout << "BACKUP SERVER OFFLINE - IT WILL BE SKIPPED" << std::endl;
            continue;
        }

        std::cout << "PRIMARY IS SENDING NEW FOLLOWER TO BACKUP AT PORT " << it->first << std::endl;
        char *message = new char[MAX_MAIL_SIZE];

        message[0] = MSG_FOLLOW;

        memcpy(&message[1], follower.c_str(), MAX_DATA_SIZE);
        memcpy(&message[1 + MAX_DATA_SIZE], followed.c_str(), MAX_DATA_SIZE);

        n = write(sockfd, message, MAX_MAIL_SIZE);

        if (n < 0)
            Logger.write_error("Failed to write to socket");
    }
}

void ReplicManager::notify_pending_notification(std::string followed, notification current_notification)
{
    for (auto it = rm->begin(); it != rm->end(); it++)
    {
        int sockfd, n;
        struct sockaddr_in backup_server_addr;

        backup_server_addr.sin_family = AF_INET;
        backup_server_addr.sin_port = htons(it->first);
        backup_server_addr.sin_addr = (it->second).sin_addr;
        bzero(&(backup_server_addr.sin_zero), 8);

        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        {
            Logger.write_error("ERROR: Opening socket");
            continue;
        }

        if (connect(sockfd, (struct sockaddr *)&backup_server_addr, sizeof(backup_server_addr)) < 0)
        {
            std::cout << "BACKUP SERVER OFFLINE - IT WILL BE SKIPPED" << std::endl;
            continue;
        }

        std::cout << "PRIMARY IS SENDING NEW PENDING NOTIFICATION TO BACKUP AT PORT " << it->first << std::endl;
        char *message = new char[MAX_MAIL_SIZE];

        message[0] = MSG_NEW_PENDING_NOTIFICATION;

        memcpy(&message[1], followed.c_str(), MAX_DATA_SIZE);

        // Notification - owner
        memcpy(&message[1 + MAX_DATA_SIZE], current_notification.owner.c_str(), MAX_DATA_SIZE);

        // Notification - timestamp (uint32_t)
        uint32_t timestampNormalized = htonl(current_notification.timestamp);

        message[1 + 2 * MAX_DATA_SIZE + 0] = timestampNormalized >> 24; // MSB
        message[1 + 2 * MAX_DATA_SIZE + 1] = timestampNormalized >> 16;
        message[1 + 2 * MAX_DATA_SIZE + 2] = timestampNormalized >> 8;
        message[1 + 2 * MAX_DATA_SIZE + 3] = timestampNormalized;

        // Notification - _message
        memcpy(&message[1 + 2 * MAX_DATA_SIZE + 4], current_notification._message.c_str(), MAX_DATA_SIZE);

        n = write(sockfd, message, MAX_MAIL_SIZE);

        if (n < 0)
            Logger.write_error("Failed to write to socket");
    }
}

void ReplicManager::notify_new_session(client_session session)
{
    for (auto it = rm->begin(); it != rm->end(); it++)
    {
        int sockfd, n;
        struct sockaddr_in backup_server_addr;

        backup_server_addr.sin_family = AF_INET;
        backup_server_addr.sin_port = htons(it->first);
        backup_server_addr.sin_addr = (it->second).sin_addr;
        bzero(&(backup_server_addr.sin_zero), 8);

        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        {
            Logger.write_error("ERROR: Opening socket");
            continue;
        }

        if (connect(sockfd, (struct sockaddr *)&backup_server_addr, sizeof(backup_server_addr)) < 0)
        {
            std::cout << "BACKUP SERVER OFFLINE - IT WILL BE SKIPPED" << std::endl;
            continue;
        }

        std::cout << "PRIMARY IS SENDING COOKIES TO BACKUP AT PORT " << it->first << std::endl;
        char *message = new char[MAX_MAIL_SIZE];

        message[0] = MSG_NEW_SESSION;

        // COOKIE
        memcpy(&message[1], session.session_id.c_str(), COOKIE_LENGTH);

        // IP
        memcpy(&message[1 + COOKIE_LENGTH], session.ip.c_str(), 16);

        // NOTIFICATION PORT
        memcpy(&message[1 + COOKIE_LENGTH + 16], session.notification_port.c_str(), 6);

        n = write(sockfd, message, MAX_MAIL_SIZE);

        if (n < 0)
            Logger.write_error("Failed to write to socket");
    }
}

void ReplicManager::notify_new_backup()
{
    for (auto it = rm->begin(); it != rm->end(); it++)
    {
        int sockfd, n;
        struct sockaddr_in backup_server_addr;

        backup_server_addr.sin_family = AF_INET;
        backup_server_addr.sin_port = htons(it->first);
        backup_server_addr.sin_addr = (it->second).sin_addr;
        bzero(&(backup_server_addr.sin_zero), 8);

        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        {
            Logger.write_error("ERROR: Opening socket");
            continue;
        }

        if (connect(sockfd, (struct sockaddr *)&backup_server_addr, sizeof(backup_server_addr)) < 0)
        {
            std::cout << "BACKUP SERVER OFFLINE - IT WILL BE SKIPPED" << std::endl;
            continue;
        }

        std::cout << "PRIMARY IS SENDING NEW AVAILABLE PORT TO BACKUP AT PORT " << it->first << std::endl;
        char *message = new char[MAX_MAIL_SIZE];

        message[0] = MSG_NEW_AVAILABLE_PORT;
        message[1] = htons(g_availablePort) >> 8; //MSB
        message[2] = htons(g_availablePort);      // LSB

        n = write(sockfd, message, MAX_MAIL_SIZE);

        if (n < 0)
            Logger.write_error("Failed to write to socket");
    }
}

void ReplicManager::add_new_backup_server(sockaddr_in cli_addr)
{
    rm->insert(std::make_pair(g_availablePort, cli_addr));
    std::cout << "NEW BACKUP SERVER ADDED AT PORT " << g_availablePort << std::endl;

    g_availablePort++;

    std::cout << "SENDING AVAILABLE PORT FOR EACH BACKUP SERVER..." << std::endl;
    notify_new_backup();
    std::cout << "DONE!" << std::endl;

    std::cout << "SENDING BACKUP LIST FOR EACH BACKUP SERVER..." << std::endl;
    notify_list_backups(rm);
    std::cout << "DONE!" << std::endl;
}

void ReplicManager::config_new_backup_server(int connection_socket, sockaddr_in cli_addr)
{
    uint16_t availablePort = g_availablePort;
    uint16_t bigEndian = htons(availablePort);
    std::cout << "PRIMARY: AVAILABLE PORT: " << availablePort << std::endl;
    char *message = new char[MAX_MAIL_SIZE];
    message[0] = bigEndian >> 8; // MSB
    message[1] = bigEndian;      // LSB

    int n = write(connection_socket, message, MAX_MAIL_SIZE);
    if (n < 0)
        std::cout << "PRIMARY: Failed to write to socket" << std::endl;
    else
    {
        add_new_backup_server(cli_addr);

        g_availablePort++;
    }

    close(connection_socket);
}

void ReplicManager::service_new_backup()
{
    int server_socket;
    socklen_t clilen;
    struct sockaddr_in serv_addr;
    std::string input;
    int connection_socket;
    struct sockaddr_in cli_addr;

    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        std::cout << "erro socket";
        exit(-1);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5000); // PORTA 5000: FIXA PARA RECEPCIONAR NOVOS BACKUPS PELO PRIMARIO
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    bzero(&(serv_addr.sin_zero), 8);

    if (bind(server_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        std::cout << "erro bind new backup";
        exit(-1);
    }

    listen(server_socket, MAX_WAITING_BACKUPS);

    clilen = sizeof(struct sockaddr_in);

    while (true)
    {

        if ((connection_socket = accept(server_socket, (struct sockaddr *)&cli_addr, &clilen)) == -1)
        {
            continue;
        }

        std::thread backup_server_thread = std::thread(config_new_backup_server, connection_socket, cli_addr);
        backup_server_thread.detach();
    }

    close(server_socket);
}

/// Send primary ip and port to all backups
void ReplicManager::notify_primary_addr()
{
    struct hostent *server_host;
    struct in_addr addr;

    inet_aton(primaryIp.c_str(), &addr);
    server_host = gethostbyaddr(&addr, sizeof(primaryIp), AF_INET);

    if (server_host == NULL)
    {
        Logger.write_error("No such host!");
        exit(-1);
    }

    for (auto it = rm->begin(); it != rm->end(); it++)
    {
        int sockfd, n;

        struct sockaddr_in backup_server_addr;

        backup_server_addr.sin_family = AF_INET;
        backup_server_addr.sin_port = htons(it->first);
        backup_server_addr.sin_addr = (it->second).sin_addr;
        bzero(&(backup_server_addr.sin_zero), 8);

        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        {
            Logger.write_error("ERROR: Opening socket");
            continue;
        }

        if (connect(sockfd, (struct sockaddr *)&backup_server_addr, sizeof(backup_server_addr)) < 0)
        {
            std::cout << "BACKUP SERVER OFFLINE - IT WILL BE SKIPPED" << std::endl;
            continue;
        }

        std::cout << "PRIMARY IS SENDING ITS ADDRESS TO BACKUP AT PORT " << it->first << std::endl;
        char *message = new char[MAX_MAIL_SIZE];

        uint32_t normalizedPrimaryIp = htonl(addr.s_addr);
        uint16_t normalizedPrimaryPORT_HEARTBEAT = htons(PORT_HEARTBEAT);

        message[0] = MSG_PRIMARY_ADDR;

        // IP
        message[1] = normalizedPrimaryIp >> 24; //MSB
        message[2] = normalizedPrimaryIp >> 16;
        message[3] = normalizedPrimaryIp >> 8;
        message[4] = normalizedPrimaryIp; // LSB

        // PORT
        //message[5] = normalizedPrimaryPORT_HEARTBEAT >> 8; // MSB
        //message[6] = normalizedPrimaryPORT_HEARTBEAT;      // LSB

        n = write(sockfd, message, MAX_MAIL_SIZE);

        if (n < 0)
            Logger.write_error("Failed to write to socket");
    }
}



//
//      BACKUP SERVER
//

uint16_t ReplicManager::ask_primary_available_port()
{
    int sockfd, n;
    struct sockaddr_in server_addr;
    struct hostent *server_host;
    struct in_addr addr;

    inet_aton(primaryIp.c_str(), &addr);
    server_host = gethostbyaddr(&addr, sizeof(primaryIp), AF_INET);

    if (server_host == NULL)
    {
        Logger.write_error("No such host!");
        exit(0);
    }

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        Logger.write_error("Opening socket");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(5000);
    server_addr.sin_addr = *((struct in_addr *)server_host->h_addr);
    bzero(&(server_addr.sin_zero), 8);

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
        Logger.write_error("Erro - Connecting - available port");

    char buffer_response[MAX_MAIL_SIZE];
    bzero(buffer_response, MAX_MAIL_SIZE);

    n = read(sockfd, buffer_response, MAX_MAIL_SIZE);
    if (n < 0)
        Logger.write_error("Reading from socket");

    uint16_t port = ntohs(buffer_response[0] << 8 | buffer_response[1]);

    close(sockfd);

    return port;
}

void ReplicManager::receive_primary_addr()
{
    std::cout << "BACKUP(" << getpid() << ") WAITING SERVER CONNECTION" << std::endl;

    connection_socket = accept(server_socket, (struct sockaddr *)&cli_addr, &clilen);

    if (connection_socket == -1)
    {
        std::cout << "BACKUP(" << getpid() << ") Connection failed" << std::endl;
        exit(-1);
    }

    std::cout << "BACKUP(" << getpid() << ") CONNECTION OK" << std::endl;
    std::cout << "BACKUP(" << getpid() << ") WAITING SERVER SEND A MESSAGE" << std::endl;

    readBytesFromSocket = read(connection_socket, buffer_response, MAX_MAIL_SIZE);

    if (readBytesFromSocket < 0)
    {
        fprintf(stderr, "socket() failed: %s\n", strerror(errno));
        Logger.write_error("ERROR: Reading from socket");
        close(connection_socket);
        exit(-1);
    }

    if (buffer_response[0] == MSG_PRIMARY_ADDR)
    {
        primaryIp = ntohl(
            buffer_response[1] << 24 
            | buffer_response[2] << 16 
            | buffer_response[3] << 8 
            | buffer_response[4]
        );

        //portHeartbeat = ntohl(
        //    buffer_response[5] << 8 | buffer_response[6]);
    }
    else
    {
        std::cout << "BACKUP(" << getpid() << ") RECEIVED FROM PRIMARY: UNKNOWN MESSAGE" << std::endl;
    }
}

void ReplicManager::init_server_as_backup()
{
    std::cout << "CONFIGURING SERVER TO BE BACKUP" << std::endl;

    uint16_t serverPort = ask_primary_available_port();

    std::cout << "BACKUP(" << getpid() << ") - PRIMARY GIVE ME THE PORT " << serverPort << std::endl;

    struct sockaddr_in serv_addr;
    std::string input;
    bzero(buffer_response, MAX_MAIL_SIZE);

    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        std::cout << "erro socket";
        exit(-1);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(serverPort);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    bzero(&(serv_addr.sin_zero), 8);

    if (bind(server_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        int option = 1;
        if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) < 0)
        {
            std::cout << "erro bind - backup server";
            fprintf(stderr, "socket() failed: %s\n", strerror(errno));
            exit(-1);
        }
    }

    listen(server_socket, 1);

    while (true)
    {
        std::cout << "BACKUP(" << getpid() << ") WAITING SERVER CONNECTION" << std::endl;

        bool timeout = Scheduler::set_timeout_to_routine([]() {
            connection_socket = accept(server_socket, (struct sockaddr *)&cli_addr, &clilen);
        }, TIMEOUT_SERVER_MS);

        if (timeout)
            break;

        if (connection_socket == -1)
            continue;

        std::cout << "BACKUP(" << getpid() << ") CONNECTION OK" << std::endl;
        std::cout << "BACKUP(" << getpid() << ") WAITING SERVER SEND A MESSAGE" << std::endl;

        timeout = Scheduler::set_timeout_to_routine([]() {
            readBytesFromSocket = read(connection_socket, buffer_response, MAX_MAIL_SIZE);
        }, TIMEOUT_SERVER_MS);

        if (timeout)
            break;

        if (readBytesFromSocket < 0)
        {
            fprintf(stderr, "socket() failed: %s\n", strerror(errno));
            Logger.write_error("ERROR: Reading from socket");
            close(connection_socket);
            continue;
        }

        if (buffer_response[0] == MSG_HEARTBEAT)
            std::cout << "BACKUP(" << getpid() << ") RECEIVED FROM PRIMARY: HEARTBEAT" << std::endl;
        else if (buffer_response[0] == MSG_NEW_AVAILABLE_PORT)
        {
            uint16_t availablePort = ntohs(buffer_response[1] << 8 | buffer_response[2]);
            g_availablePort = availablePort;

            std::cout << "BACKUP(" << getpid() << ") RECEIVED FROM PRIMARY: " << g_availablePort << std::endl;
        }
        else if (buffer_response[0] == MSG_NEW_SESSION)
        {
            char cookie[COOKIE_LENGTH];
            char ip[16];
            char port[6];

            // COOKIE
            memcpy(&cookie, &buffer_response[1], COOKIE_LENGTH);

            // IP
            memcpy(&ip, &buffer_response[1 + COOKIE_LENGTH], 16);

            // NOTIFICATION PORT
            memcpy(&port, &buffer_response[1 + COOKIE_LENGTH + 16], 6);

            std::cout << "BACKUP(" << getpid() << ") RECEIVED FROM PRIMARY: SESSION" << std::endl;

            // TODO: Store session at server communication manager
        }
        else if (buffer_response[0] == MSG_NEW_PENDING_NOTIFICATION)
        {
            char followed[MAX_DATA_SIZE];
            uint32_t timestampNormalized;
            notification n;

            memcpy(&followed, &buffer_response[1], MAX_DATA_SIZE);

            timestampNormalized = ntohl(
                buffer_response[1 + MAX_DATA_SIZE + 0] << 24 
                | buffer_response[1 + MAX_DATA_SIZE + 1] << 16 
                | buffer_response[1 + MAX_DATA_SIZE + 2] << 8 
                | buffer_response[1 + MAX_DATA_SIZE + 3]
            );

            memcpy(&n._message, &buffer_response[1 + 2 * MAX_DATA_SIZE + 4], MAX_DATA_SIZE);

            n.timestamp = ntohl(timestampNormalized);

            std::cout << "BACKUP(" << getpid() << ") RECEIVED FROM PRIMARY: NEW PENDING NOTIFICATION" << std::endl;

            // TODO: store new pending notification at server notification manager
        }
        else if (buffer_response[0] == MSG_FOLLOW)
        {
            char follower[MAX_DATA_SIZE];
            char followed[MAX_DATA_SIZE];

            memcpy(&follower, &buffer_response[1], MAX_DATA_SIZE);
            memcpy(&followed, &buffer_response[1 + MAX_DATA_SIZE], MAX_DATA_SIZE);

            std::cout << "BACKUP(" << getpid() << ") RECEIVED FROM PRIMARY: FOLLOW" << std::endl;

            // TODO: send new follower to profile session manager
        }
        else if (buffer_response[0] == MSG_CLOSE_SESSION)
        {
            char cookie[COOKIE_LENGTH];
            char ip[16];
            char port[6];

            // COOKIE
            memcpy(&cookie, &buffer_response[1], COOKIE_LENGTH);

            // IP
            memcpy(&ip, &buffer_response[1 + COOKIE_LENGTH], 16);

            // NOTIFICATION PORT
            memcpy(&port, &buffer_response[1 + COOKIE_LENGTH + 16], 6);

            std::cout << "BACKUP(" << getpid() << ") RECEIVED FROM PRIMARY: CLOSE SESSION" << std::endl;

            // TODO: send session to server communication manager
        }
        else if (buffer_response[0] == MSG_LIST_BACKUP)
        {
            uint8_t totalBackups = buffer_response[1];
            std::map<int, sockaddr_in> *rms = new std::map<int, sockaddr_in>();

            for (int i = 0; i < totalBackups; i++)
            {
                uint32_t port;
                sockaddr_in server;

                memcpy(&port, &buffer_response[2 + i * sizeof(sockaddr_in)], 4);
                port = ntohl(
                    buffer_response[2 + i * sizeof(sockaddr_in) + 0] << 24 
                    | buffer_response[2 + i * sizeof(sockaddr_in) + 1] << 16 
                    | buffer_response[2 + i * sizeof(sockaddr_in) + 2] << 8 
                    | buffer_response[2 + i * sizeof(sockaddr_in) + 3]
                );

                if (port != serverPort)
                {
                    memcpy(&server, &buffer_response[2 + i * sizeof(sockaddr_in) + 4], sizeof(sockaddr_in));

                    rms->insert(std::make_pair(port, server));
                }
            }

            rm = rms;

            std::cout << "BACKUP(" << getpid() << ") RECEIVED FROM PRIMARY: BACKUP LIST" << std::endl;

            for (auto it = rms->begin(); it != rms->end(); it++)
            {
                std::cout << it->first << ": " << (it->second).sin_addr.s_addr << "; " << (it->second).sin_port << std::endl;
            }
            std::cout << std::endl;
        }
        else
        {
            std::cout << "BACKUP(" << getpid() << ") RECEIVED FROM PRIMARY: UNKNOWN MESSAGE" << std::endl;
        }
    }

    close(connection_socket);
    close(server_socket);

    std::cout << "BACKUP(" << getpid() << ") PRIMARY OFFLINE - I'M STARTING ELECTION LEADER" << std::endl;

    if (true)
    {
        std::cout << "BACKUP(" << getpid() << ") I'M THE LEADER!!!" << std::endl;
        std::cout << "BACKUP(" << getpid() << ") I AM PRIMARY :D ! HAHAHAH" << std::endl;
        std::cout << "BACKUP(" << getpid() << ") I'LL SEND MY ADDRESS TO BACKUPS" << std::endl;

        notify_primary_addr();

        init_server_as_primary();
    }
    else
    {
        std::cout << "BACKUP(" << getpid() << ") I'M BACKUP AGAIN ;(" << std::endl;
        std::cout << "BACKUP(" << getpid() << ") I'LL WAIT PRIMARY ADDRESS" << std::endl;

        receive_primary_addr();

        init_server_as_backup();
    }
}

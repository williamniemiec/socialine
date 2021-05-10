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
#include "../../Utils/StringUtils.h"
#include "../include/DataManager.hpp"


#define MSG_HEARTBEAT 0
#define MSG_NEW_SESSION 1
#define MSG_NEW_PENDING_NOTIFICATION 2
#define MSG_CLOSE_SESSION 3
#define MSG_FOLLOW 4
#define MSG_ELECTION 5
#define MSG_ELECTED 6
#define MSG_LIST_BACKUP 7
#define MSG_PRIMARY_ADDR 8
#define TIMEOUT_SERVER_MS 6000
#define TIMEOUT_RECEIVE_ELECTION_MS 3500
#define HEARTBEAT_MS 3000
#define MAX_WAITING_BACKUPS 5
#define PORT_MULTICAST 4001
#define PORT_HEARTBEAT 4002
#define NEW_BACKUP_PORT 6000
#define BACKUP_PORT_START 6001
#define BACKUP_PORT_END 6999

using namespace socialine::util::task;
using namespace socialine::utils;

//-------------------------------------------------------------------------
//		Attributes
//-------------------------------------------------------------------------
int ReplicManager::g_process_id = 0;
std::list<Server> *ReplicManager::rm = new std::list<Server>();
socklen_t ReplicManager::clilen = sizeof(struct sockaddr_in);
bool ReplicManager::g_primary_server_online = true;
int ReplicManager::server_socket;
int ReplicManager::connection_socket;
struct sockaddr_in ReplicManager::cli_addr;
int ReplicManager::readBytesFromSocket;
char ReplicManager::buffer_response[MAX_MAIL_SIZE];
std::string ReplicManager::primaryIp;
std::string ReplicManager::multicastIp = "226.1.1.1";
int ReplicManager::myPid = getpid();


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

    std::thread thread_new_backup(new_backup_service);
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
                backup_server_addr.sin_port = htons(it->get_port());
                backup_server_addr.sin_addr = get_ip_by_address(it->get_ip());
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
                    rm->remove(*it);
                    break; // Avoids segmentation fault
                }

                std::cout << "PRIMARY IS SENDING HEARTBEAT TO BACKUP " << it->get_signature() << std::endl;
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

in_addr ReplicManager::get_ip_by_address(std::string address)
{
    hostent *server_host;
    struct in_addr addr;
    inet_aton(address.c_str(), &addr);
    server_host = gethostbyaddr(&addr, sizeof(address), AF_INET);

    return *((struct in_addr *)server_host->h_addr);
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

void ReplicManager::notify_list_backups(std::list<Server>* backups)
{
    for (auto it = backups->begin(); it != backups->end(); it++)
    {
        int sockfd, n;
        struct sockaddr_in backup_server_addr;

        backup_server_addr.sin_family = AF_INET;
        backup_server_addr.sin_port = htons(it->get_port());
        backup_server_addr.sin_addr = get_ip_by_address(it->get_ip());
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

        std::cout << "PRIMARY IS SENDING BACKUP LIST TO BACKUP " << it->get_signature() << std::endl;
        char *message = new char[MAX_MAIL_SIZE];

        message[0] = MSG_LIST_BACKUP;

        uint8_t totalBackups = backups->size();
        int i = 0;

        message[1] = totalBackups;

        for (auto it = backups->begin(); it != backups->end(); it++)
        {
            uint16_t normalizedPort = htons(it->get_port());
            std::string serverIp = it->get_ip();

            message[2 + i * (18+sizeof(int)) + 0] = normalizedPort >> 8; // MSB
            message[2 + i * (18+sizeof(int)) + 1] = normalizedPort; // LSB

            memcpy(&message[2 + i * (18+sizeof(int)) + 2], serverIp.c_str(), 16);

            int serverPid = it->get_pid();

            memcpy(&message[2 + i * (18+sizeof(int)) + 16], &serverPid, sizeof(int));


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
        backup_server_addr.sin_port = htons(it->get_port());
        backup_server_addr.sin_addr = get_ip_by_address(it->get_ip());
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

        std::cout << "PRIMARY IS SENDING CLOSE SESSION TO BACKUP " << it->get_signature() << std::endl;
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
        send_follow(*it, follower, followed);
    }
}

void ReplicManager::send_follow(Server server, std::string follower, std::string followed)
{
    int sockfd, n;
    struct sockaddr_in backup_server_addr;

    backup_server_addr.sin_family = AF_INET;
    backup_server_addr.sin_port = htons(server.get_port());
    backup_server_addr.sin_addr = get_ip_by_address(server.get_ip());
    bzero(&(backup_server_addr.sin_zero), 8);

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        Logger.write_error("ERROR: Opening socket");
        return;
    }

    if (connect(sockfd, (struct sockaddr *)&backup_server_addr, sizeof(backup_server_addr)) < 0)
    {
        std::cout << "BACKUP SERVER OFFLINE - IT WILL BE SKIPPED" << std::endl;
        return;
    }

    std::cout << "PRIMARY IS SENDING NEW FOLLOWER TO BACKUP " << server.get_signature() << std::endl;
    char *message = new char[MAX_MAIL_SIZE];

    message[0] = MSG_FOLLOW;

    memcpy(&message[1], follower.c_str(), MAX_DATA_SIZE);
    memcpy(&message[1 + MAX_DATA_SIZE], followed.c_str(), MAX_DATA_SIZE);

    n = write(sockfd, message, MAX_MAIL_SIZE);

    if (n < 0)
        Logger.write_error("Failed to write to socket");
}

void ReplicManager::notify_pending_notification(std::string followed, notification current_notification)
{
    for (auto it = rm->begin(); it != rm->end(); it++)
    {
        send_pending_notification(*it, followed, current_notification);
    }
}

void ReplicManager::send_pending_notification(Server server, std::string followed, notification current_notification)
{
    int sockfd, n;
    struct sockaddr_in backup_server_addr;

    backup_server_addr.sin_family = AF_INET;
    backup_server_addr.sin_port = htons(server.get_port());
    backup_server_addr.sin_addr = get_ip_by_address(server.get_ip());
    bzero(&(backup_server_addr.sin_zero), 8);

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        Logger.write_error("ERROR: Opening socket");
        return;
    }

    if (connect(sockfd, (struct sockaddr *)&backup_server_addr, sizeof(backup_server_addr)) < 0)
    {
        std::cout << "BACKUP SERVER OFFLINE - IT WILL BE SKIPPED" << std::endl;
        return;
    }

    std::cout << "PRIMARY IS SENDING NEW PENDING NOTIFICATION TO BACKUP " << server.get_signature() << std::endl;
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

void ReplicManager::notify_new_session(std::string sessionId, client_session session)
{
    for (auto it = rm->begin(); it != rm->end(); it++)
    {
        send_session(*it, sessionId, session);
    }
}

void ReplicManager::send_session(Server server, std::string sessionId, client_session session)
{
    int sockfd, n;
    struct sockaddr_in backup_server_addr;

    backup_server_addr.sin_family = AF_INET;
    backup_server_addr.sin_port = htons(server.get_port());
    backup_server_addr.sin_addr = get_ip_by_address(server.get_ip());
    bzero(&(backup_server_addr.sin_zero), 8);

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        Logger.write_error("ERROR: Opening socket");
        return;
    }

    if (connect(sockfd, (struct sockaddr *)&backup_server_addr, sizeof(backup_server_addr)) < 0)
    {
        std::cout << "BACKUP SERVER OFFLINE - IT WILL BE SKIPPED" << std::endl;
        return;
    }

    std::cout << "PRIMARY IS SENDING COOKIES TO BACKUP " << server.get_signature() << std::endl;
    char *message = new char[MAX_MAIL_SIZE];

    message[0] = MSG_NEW_SESSION;

    // COOKIE
    memcpy(&message[1], sessionId.c_str(), COOKIE_LENGTH);

    // IP
    memcpy(&message[1 + COOKIE_LENGTH], session.ip.c_str(), 16);

    // NOTIFICATION PORT
    memcpy(&message[1 + COOKIE_LENGTH + 16], session.notification_port.c_str(), 6);

    n = write(sockfd, message, MAX_MAIL_SIZE);

    if (n < 0)
        Logger.write_error("Failed to write to socket");
}

void ReplicManager::add_new_backup_server(std::string ip, uint16_t port, int pid)
{
    rm->push_back(Server(ip, port, pid));
    std::cout << "NEW BACKUP (" << pid << ") SERVER ADDED: " << ip << ":" << port << std::endl;

    std::cout << "SENDING BACKUP LIST FOR EACH BACKUP SERVER..." << std::endl;
    notify_list_backups(rm);
    std::cout << "DONE!" << std::endl;
}

void ReplicManager::config_new_backup_server(int connection_socket, sockaddr_in cli_addr)
{
    char *message = new char[MAX_MAIL_SIZE];
 
    int n = read(connection_socket, message, MAX_MAIL_SIZE);
    if (n < 0)
        std::cout << "PRIMARY: Failed to write to socket" << std::endl;
    else
    {
        std::vector<std::string> body = StringUtils::split(std::string(message), ";");
        
        add_new_backup_server(
            body[0], 
            static_cast<uint16_t>(std::stoi(body[1])),
            static_cast<uint32_t>(std::stoi(body[2]))
        );
    }

    close(connection_socket);
}

void ReplicManager::new_backup_service()
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
    serv_addr.sin_port = htons(NEW_BACKUP_PORT); 
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    bzero(&(serv_addr.sin_zero), 8);

    int option = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) < 0)
    {
        std::cout << "erro bind new backup";
        fprintf(stderr, "socket() failed: %s\n", strerror(errno));
        exit(-1);
    }

    if (bind(server_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        std::cout << "FALHA NEW BACKUP SERVICE" << std::endl;
        fprintf(stderr, "socket() failed: %s\n", strerror(errno));
        exit(-1);
    }

    listen(server_socket, MAX_WAITING_BACKUPS);

    clilen = sizeof(struct sockaddr_in);

    while (true)
    {
        if ((connection_socket = accept(server_socket, (struct sockaddr *)&cli_addr, &clilen)) == -1)
            continue;

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
        backup_server_addr.sin_port = htons(it->get_port());
        backup_server_addr.sin_addr = get_ip_by_address(it->get_ip());
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

        std::cout << "PRIMARY IS SENDING ITS ADDRESS TO BACKUP " << it->get_signature() << std::endl;
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

void ReplicManager::send_all_sessions(Server target)
{
    std::cout << "SEND ALL SESSIONS TO BACKUP " << target.get_signature() << std::endl;

    std::unordered_map<std::string, client_session> sessions = DataManager::get_all_sessions();
    
    for (auto it = sessions.begin(); it != sessions.end(); it++)
    {
        send_session(target, it->first, it->second);
    }

    std::cout << "DONE" << std::endl;
}

void ReplicManager::send_all_followers(Server target)
{
    std::cout << "SEND ALL FOLLOWERS TO BACKUP " << target.get_signature() << std::endl;

    std::unordered_map<std::string, std::vector<std::string>> followers = DataManager::get_all_followers();

    for (auto it = followers.begin(); it != followers.end(); it++)
    {
        std::vector<std::string> followers = it->second;

        for (std::string user : followers)
        {
            send_follow(target, it->first, user);
        }
    }

    std::cout << "DONE" << std::endl;
}

void ReplicManager::send_all_pending_notifications(Server target)
{
    std::cout << "SEND ALL PENDING NOTIFICATIONS TO BACKUP " << target.get_signature() << std::endl;

    std::unordered_map<std::string, std::vector<notification>> pendingNotifications = DataManager::get_all_pending_notifications();

    for (auto it = pendingNotifications.begin(); it != pendingNotifications.end(); it++)
    {
        std::vector<notification> notifications = it->second;

        for (notification notification : notifications)
        {
            send_pending_notification(target, it->first, notification);
        }
    }

    std::cout << "DONE" << std::endl;
}



//
//      BACKUP SERVER
//

bool ReplicManager::is_port_available(uint16_t port)
{
    bool isAvailable = true;
    int server_socket;
    struct sockaddr_in serv_addr;
    std::string input;

    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        isAvailable = false;
    }
    else
    {
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(port);
        serv_addr.sin_addr.s_addr = INADDR_ANY;

        bzero(&(serv_addr.sin_zero), 8);

        if (bind(server_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
            isAvailable = false;
    }

    close(server_socket);
    sleep(1);
    
    return isAvailable;
}

uint16_t ReplicManager::get_available_port_in_range(uint16_t start, uint16_t end)
{
    bool foundAvailablePort = false;
    uint16_t currentPort = start;

    while (!foundAvailablePort && (currentPort <= end))
    {
        if (is_port_available(currentPort))
            foundAvailablePort = true;
        else
            currentPort++;
    }
    
    return (currentPort > end) ? -1 :currentPort;
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

void ReplicManager::connect_with_primary_server(std::string backupIp, uint16_t backupPort)
{
    int sockfd, n;
    struct sockaddr_in backup_server_addr;

    backup_server_addr.sin_family = AF_INET;
    backup_server_addr.sin_port = htons(NEW_BACKUP_PORT);
    backup_server_addr.sin_addr = get_ip_by_address(primaryIp);
    bzero(&(backup_server_addr.sin_zero), 8);

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        Logger.write_error("ERROR: Opening socket");
        exit(-1);
    }

    if (connect(sockfd, (struct sockaddr *)&backup_server_addr, sizeof(backup_server_addr)) < 0)
    {
        fprintf(stderr, "connect failed: %s\n", strerror(errno));
        std::cout << "CONNECTION WITH PRIMARY SERVER FAILED!" << std::endl;
        std::cout << "PRIMARY IP: " << primaryIp << std::endl;
        exit(-1);
    }

    std::cout << "BACKUP IS SENDING ITS ADDRESS TO PRIMARY" << std::endl;
   
    std::string body = backupIp + ";" + std::to_string(backupPort) + ";" + std::to_string(myPid);

    n = write(sockfd, body.c_str(), MAX_MAIL_SIZE);

    if (n < 0)
        Logger.write_error("Failed to write to socket");
}

void ReplicManager::init_server_as_backup()
{
    bool is_backup_server = true;
    std::cout << "CONFIGURING SERVER TO BE BACKUP" << std::endl;

    std::string serverIp = get_local_ip();
    uint16_t serverPort = get_available_port_in_range(BACKUP_PORT_START, BACKUP_PORT_END);

    std::cout << "BACKUP(" << getpid() << ") MY IP IS: " << serverIp << std::endl;
    std::cout << "BACKUP(" << getpid() << ") MY PORT IS: " << serverPort << std::endl;

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
        std::cout << "erro bind - backup server";
        fprintf(stderr, "socket() failed: %s\n", strerror(errno));
        exit(-1);
    }

    listen(server_socket, 1);

    connect_with_primary_server(serverIp, serverPort);

    while (is_backup_server)
    {
        while (true)
        {
            std::cout << "BACKUP(" << getpid() << ") WAITING SERVER CONNECTION" << std::endl;

            bool timeout = Scheduler::set_timeout_to_routine([]() 
            {
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
                std::list<Server> *rms = new std::list<Server>();

                for (int i = 0; i < totalBackups; i++)
                {
                    uint16_t backupPort;
                    backupPort = ntohs(
                        buffer_response[2 + i * (18+sizeof(int)) + 0] << 8 
                        | buffer_response[2 + i * (18+sizeof(int)) + 1]
                    );

                    if (backupPort != serverPort)
                    {
                        char server[16];
                        memcpy(&server, &buffer_response[2 + i * (18+sizeof(int)) + 2], 16);

                        int serverPid;
                        memcpy(&serverPid, &buffer_response[2 + i * (18+sizeof(int)) + 16], sizeof(int));

                        rms->push_back(Server(server, backupPort, serverPid));
                    }
                }

                rm = rms;

                std::cout << "BACKUP(" << getpid() << ") RECEIVED FROM PRIMARY: BACKUP LIST" << std::endl;

                for (Server & replic : *rms) {
                    std::cout << replic.get_signature() << std::endl;
                }
            }
            else
            {
                std::cout << "BACKUP(" << getpid() << ") RECEIVED FROM PRIMARY: UNKNOWN MESSAGE" << std::endl;
            }
        }

        close(connection_socket);
        close(server_socket);

        std::cout << "BACKUP(" << getpid() << ") PRIMARY OFFLINE - I'M STARTING ELECTION LEADER" << std::endl;

        //is_backup_server = !start_election_leader(Server(serverIp, serverPort, myPid));
        is_backup_server = false;
        if (is_backup_server)
        {
            std::cout << "BACKUP(" << getpid() << ") I'M BACKUP AGAIN ;(" << std::endl;
            std::cout << "BACKUP(" << getpid() << ") I'LL WAIT PRIMARY ADDRESS" << std::endl;

            receive_primary_addr();
            init_server_as_backup();
        }
        else
        {
            std::cout << "BACKUP(" << getpid() << ") I'M THE LEADER!!!" << std::endl;
            std::cout << "BACKUP(" << getpid() << ") I AM PRIMARY :D ! HAHAHAH" << std::endl;
            std::cout << "BACKUP(" << getpid() << ") I'LL SEND MY ADDRESS TO BACKUPS" << std::endl;

            notify_primary_addr();
            init_server_as_primary();
        }
    }
}

Server ReplicManager::get_server_with_pid(int pid)
{
    for (Server s : *rm) 
    {
        if (s.get_pid() == pid)
            return s;
    }

    return Server("", 0, 0);
}

std::map<int, std::list<Server>> ReplicManager::get_servers_ordered_by_pid_ascending()
{
    std::map<int, std::list<Server>> servers;

    for (Server server : *rm)
    {
        if (servers.find(server.get_pid()) == servers.end())
        {
            std::list<Server> servers_with_same_pid;
            servers_with_same_pid.push_back(server);

            servers.insert(std::make_pair(server.get_pid(), servers_with_same_pid));
        }
        else
        {
            servers[server.get_pid()].push_back(server);
        }
    }

    return servers;
}

bool ReplicManager::start_election_leader(Server starter)
{
    bool is_leader = false;
    std::map<int, std::list<Server>> servers_mapped_by_pid = get_servers_ordered_by_pid_ascending();
    int leader_pid = -1;
    bool participant = false;

    // First round
    

    // Second round    


    return is_leader;
}

int ReplicManager::receive_election_leader(Server receiver)
{
    int pid = -1;
    char* buffer[MAX_MAIL_SIZE];
    struct sockaddr_in serv_addr;
    struct in_addr addr;
    hostent *server_host;
    
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        std::cout << "erro socket";
        exit(-1);
    }

    inet_aton(receiver.get_ip().c_str(), &addr);
    server_host = gethostbyaddr(&addr, sizeof(receiver.get_ip()), AF_INET);

    if (server_host == NULL)
    {
        Logger.write_error("No such host!");
        exit(-1);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(receiver.get_port());
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

    bool timeout = Scheduler::set_timeout_to_routine([]() 
    {
        connection_socket = accept(server_socket, (struct sockaddr *)&cli_addr, &clilen);
    }, TIMEOUT_RECEIVE_ELECTION_MS);

    if (timeout)
        exit(-1);

    if (connection_socket == -1)
        exit(-1);

    timeout = Scheduler::set_timeout_to_routine([]() {
        readBytesFromSocket = read(connection_socket, buffer_response, MAX_MAIL_SIZE);
    }, TIMEOUT_SERVER_MS);

    if (timeout)
        exit(-1);

    if (readBytesFromSocket < 0)
    {
        fprintf(stderr, "socket() failed: %s\n", strerror(errno));
        Logger.write_error("ERROR: Reading from socket");
        close(connection_socket);
        exit(-1);
    }

    if (buffer_response[0] == MSG_ELECTION)
    {
        pid = ntohl(
            buffer_response[1] << 24
            | buffer_response[2] << 16
            | buffer_response[3] << 8
            | buffer_response[4]
        );
    }

    close(connection_socket);
    close(server_socket);

    return pid;
}

void ReplicManager::send_election_leader(int message_type, int pid, Server to)
{
    int sockfd, n;
    struct sockaddr_in backup_server_addr;

    backup_server_addr.sin_family = AF_INET;
    backup_server_addr.sin_port = htons(to.get_port());
    backup_server_addr.sin_addr = get_ip_by_address(to.get_ip());
    bzero(&(backup_server_addr.sin_zero), 8);

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        Logger.write_error("ERROR: Opening socket");
        exit(-1);
    }

    if (connect(sockfd, (struct sockaddr *)&backup_server_addr, sizeof(backup_server_addr)) < 0)
    {
        fprintf(stderr, "socket() failed: %s\n", strerror(errno));
        exit(-1); 
    }

    char *message = new char[MAX_MAIL_SIZE];
    uint32_t normalizedPid = htonl(pid);

    message[0] = message_type;

    message[1] = normalizedPid >> 24;
    message[2] = normalizedPid >> 16;
    message[3] = normalizedPid >> 8;
    message[4] = normalizedPid;

    n = write(sockfd, message, MAX_MAIL_SIZE);
    if (n < 0)
        Logger.write_error("Failed to write to socket");
}


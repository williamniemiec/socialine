#include "../include/ServerCommunicationManager.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <csignal>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>
#include <netdb.h>
#include <ctime>
#include <pthread.h>

using namespace socialine::utils;

//-------------------------------------------------------------------------
//		Global variables
//-------------------------------------------------------------------------
int SELECTED_SERVER_PORT = 0;
int SERVER_PORTS[] = {4021, 4022, 4023, 4024, 4025, 4026, 4027, 4028, 4029};
int SERVER_BROADCAST_PORTS[] = {4010, 4011, 4012, 4013, 4014, 4015, 4016, 4017, 4018, 4019, 4020};


//-------------------------------------------------------------------------
//		Attributes
//-------------------------------------------------------------------------
std::unordered_map<std::string, client_session> ServerCommunicationManager::client_sessions;
bool ServerCommunicationManager::isPrimaryServer;
ReplicManager *ServerCommunicationManager::replic_manager;


//-------------------------------------------------------------------------
//		Constructor
//-------------------------------------------------------------------------
ServerCommunicationManager::ServerCommunicationManager()
{
    replic_manager = ReplicManager::get_instance();
    replic_manager->attach(this);
    observers = std::list<IObserver *>();
    observers.push_back(replic_manager);
    isPrimaryServer = false;
}


//-------------------------------------------------------------------------
//		Methods
//-------------------------------------------------------------------------
void ServerCommunicationManager::attach(IObserver *observer)
{
    observers.push_back(observer);
}

void ServerCommunicationManager::detatch(IObserver *observer)
{
    observers.remove(observer);
}

void ServerCommunicationManager::notify_observers()
{
    std::list<std::string> body;

    body.push_back(arg0);
    body.push_back(arg1);
    body.push_back(arg2);
    body.push_back(arg3);

    for (IObserver *observer : observers)
    {
        std::thread([=]() {
            observer->update(this, body);
        }).detach();
    }
}

void ServerCommunicationManager::update(IObservable *observable, std::list<std::string> data)
{
    if (dynamic_cast<ReplicManager *>(observable) != nullptr)
    {
        if (data.front() == "PRIMARY")
        {
            isPrimaryServer = true;
            updateClientsWithNewPrimaryServer(nullptr);
        }
    }
}

void ServerCommunicationManager::start()
{
    initialize_replic_manager();
    signal(SIGPIPE, SIG_IGN);

    int server_socket;
    socklen_t clilen;
    struct sockaddr_in serv_addr;
    std::string input;

    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        Logger.write_error("ERROR opening socket");

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    bzero(&(serv_addr.sin_zero), 8);

    int i;
    int server_ports_length = sizeof(SERVER_PORTS) / sizeof(SERVER_PORTS[0]);
    for (i = 0; i < server_ports_length; i++)
    {
        serv_addr.sin_port = htons(SERVER_PORTS[i]);
        if (bind(server_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) >= 0)
        {
            Logger.write_error("Successfully bound to PORT " + std::to_string(SERVER_PORTS[i]));
            SELECTED_SERVER_PORT = SERVER_PORTS[i];
            break;
        }
        Logger.write_error("Failed binding to PORT " + std::to_string(SERVER_PORTS[i]) + ". Will try next port...");
    }

    Logger.write_info("Server will start listening");
    listen(server_socket, 5);

    clilen = sizeof(struct sockaddr_in);

    while (true)
    {
        int connection_socket;
        struct sockaddr_in cli_addr;

        if ((connection_socket = accept(server_socket, (struct sockaddr *)&cli_addr, &clilen)) == -1)
        {
            Logger.write_error("Error accepting request");
            continue;
        }

        makeCookie(&cli_addr);

        std::thread child_thread(&ServerCommunicationManager::start_client_thread, this, connection_socket, &cli_addr);
        child_thread.detach();
    }

    close(server_socket);
}

void ServerCommunicationManager::initialize_replic_manager()
{
    std::thread replic_manager_thread([&]() {
        replic_manager->run();
    });

    replic_manager_thread.detach();
}

void ServerCommunicationManager::start_client_thread(int connection_socket, sockaddr_in *cli_addr)
{
    int n;
    int response_code;
    char buffer[MAX_MAIL_SIZE];
    TaskManager taskManager;

    bzero(buffer, MAX_MAIL_SIZE);

    n = read(connection_socket, buffer, MAX_MAIL_SIZE);
    if (n < 0)
        Logger.write_debug("Reading data from socket\n");

    char received_packet_buffer[MAX_DATA_SIZE];
    struct __packet received_packet = {0, 0, 0, 0, NO_COOKIE, received_packet_buffer};
    received_packet.Deserialize(buffer);
    received_packet.print(std::string("RECEIVED"));

    std::string cookie;
    if (received_packet.cookie == NO_COOKIE)
        cookie = makeCookie(cli_addr);

    // CALLBACK METHOD TO HANDLE COMMAND EXECUTION
    std::string arguments = std::to_string(received_packet.timestamp) + '\n' + received_packet._payload + '\n';
    response_code = taskManager.run_command(received_packet.type, arguments, cookie); // enviar IP e porta.

    int type;
    std::string message;
    std::tuple<int, std::string> response_data = std::make_tuple(response_code, "");
    std::tie(type, message) = response_data;

    char response_packet_payload[MAX_DATA_SIZE];
    struct __packet response_packet = {0, 0, 0, 0, cookie, response_packet_payload};

    buildPacket(type, 0, message, &response_packet);

    char *response_buffer;
    response_buffer = (char *)calloc(MAX_MAIL_SIZE, sizeof(char));
    response_buffer = response_packet.Serialize();

    std::vector<std::string> args = StringUtils::split(received_packet._payload, "\n");

    auto it = client_sessions.find(cookie);
    if (it == client_sessions.end() && received_packet.type == CMD_LOGIN)
    {
        client_session new_session;
        new_session.ip = std::string(inet_ntoa(cli_addr->sin_addr));
        new_session.notification_port = args[1];
        client_sessions[cookie] = new_session;

        arg0 = "NEW_SESSION";
        arg1 = cookie;
        arg2 = new_session.ip;
        arg3 = new_session.notification_port;
        notify_observers();
    }
    else if (received_packet.type == CMD_LOGOUT)
    {
        client_sessions.erase(cookie);

        arg0 = "CLOSE_SESSION";
        arg1 = cookie;
        notify_observers();
    }

    n = write(connection_socket, response_buffer, MAX_MAIL_SIZE);
    if (n < 0)
        Logger.write_error("Failed to write to socket");

    close(connection_socket);
}

void ServerCommunicationManager::buildPacket(uint16_t type, uint16_t seqn, std::string message, struct __packet *packet)
{
    uint16_t headerLength = HEADER_LENGTH; 
    uint16_t length = headerLength + MAX_DATA_SIZE;
    uint16_t timestamp = getTimestamp();

    packet->type = type;
    packet->seqn = seqn;
    packet->length = length;
    packet->timestamp = timestamp;
    packet->_payload = message;
}

uint16_t ServerCommunicationManager::getTimestamp()
{
    time_t ti;
    ti = time(NULL);
    struct tm tm_time;
    tm_time = *localtime(&ti);

    return tm_time.tm_mday + (tm_time.tm_mon + 1) * 32 + (tm_time.tm_year - (1980 - 1900)) * 512;
}

std::string ServerCommunicationManager::makeCookie(sockaddr_in *cli_addr)
{
    char *ip_char_pointer = inet_ntoa(cli_addr->sin_addr);
    std::string ip_str(ip_char_pointer);

    char port_char_pointer[80];
    sprintf(port_char_pointer, "%u", htons(cli_addr->sin_port));
    std::string port_str(port_char_pointer);

    std::string cookie = ip_str + port_str;
    int cookie_prefix_length = cookie.length();

    if (cookie_prefix_length < COOKIE_LENGTH)
    {
        std::string random_suffix = random_string(COOKIE_LENGTH - cookie_prefix_length);
        cookie = ip_str + port_str + random_suffix;
    }
    else
    {
        int extra_chars = cookie_prefix_length - COOKIE_LENGTH;
        cookie.erase(cookie.length() - extra_chars);
    }

    return cookie;
}

std::string ServerCommunicationManager::random_string(size_t length)
{
    auto randchar = []() -> char {
        const char charset[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[rand() % max_index];
    };
    std::string str(length, 0);
    std::generate_n(str.begin(), length, randchar);
    return str;
}

void ServerCommunicationManager::sendNotification(std::string session_id, notification current_notification)
{
    client_session session = ServerCommunicationManager::client_sessions[session_id];

    Logger.write_debug("IP: " + session.ip);
    Logger.write_debug("PORT: " + session.notification_port);

    std::string payload = current_notification.owner + '\n' + std::to_string(current_notification.timestamp) + '\n' + current_notification._message + '\n';

    sendNotification(session.ip, session.notification_port, session_id, payload);
}

void ServerCommunicationManager::sendNotification(std::string receiver_ip, std::string receiver_port, std::string session_id, std::string message)
{
    int sockfd, n;
    struct sockaddr_in receiver_addr;
    struct hostent *receiver_host;
    struct in_addr addr;
    std::string buffer_out;

    inet_aton(receiver_ip.c_str(), &addr);

    receiver_host = gethostbyaddr(&addr, sizeof(receiver_ip), AF_INET);

    if (receiver_host == NULL)
        Logger.write_error("[Send Notification] No such client found!");

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        Logger.write_debug("[Send Notification] Opening socket");

    receiver_addr.sin_family = AF_INET;
    receiver_addr.sin_port = htons(std::stoi(receiver_port.c_str()));
    receiver_addr.sin_addr = *((struct in_addr *)receiver_host->h_addr);
    bzero(&(receiver_addr.sin_zero), 8);

    if (connect(sockfd, (struct sockaddr *)&receiver_addr, sizeof(receiver_addr)) < 0)
        Logger.write_debug("[Send Notification] Connecting");

    char *bufferPayload = (char *)calloc(MAX_DATA_SIZE, sizeof(char));
    packet packet_sent = {0, 0, 0, 0, session_id, bufferPayload};

    buildPacket(NOTIFICATION_TWEET, 0, message, &packet_sent);

    if (Logger.get_log_level() == LEVEL_DEBUG)
        packet_sent.print(std::string("SENT"));

    char *buffer;
    buffer = (char *)calloc(MAX_MAIL_SIZE, sizeof(char));
    buffer = packet_sent.Serialize();

    n = write(sockfd, buffer, MAX_MAIL_SIZE);
    if (n < 0)
        Logger.write_debug("[Send Notification] Writing to socket");

    close(sockfd);
}

void ServerCommunicationManager::listenForBroadcast()
{
    pthread_t ptid;
    pthread_create(&ptid, NULL, &threadListenForBroadcast, NULL);

    return;
}

std::string get_local_ip()
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
        Logger.write_error("Could not inet_ntop");
        exit(-1);
    }

    return std::string(buf);
}

void *ServerCommunicationManager::threadListenForBroadcast(void *arg)
{
    int server_socket;
    socklen_t clilen;
    struct sockaddr_in serv_addr;
    std::string input;

    if ((server_socket = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
        Logger.write_error("ERROR opening socket");

    int broadcast = 1;
    socklen_t sizeof_broadcast = sizeof(broadcast);
    if (setsockopt(server_socket, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof_broadcast) < 0)
    {
        std::cout << "Error in setting Broadcast option: " << errno << std::endl;
        close(server_socket);
        return 0;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    bzero(&(serv_addr.sin_zero), 8);

    int i;
    int server_broadcast_ports_length = sizeof(SERVER_BROADCAST_PORTS) / sizeof(SERVER_BROADCAST_PORTS[0]);
    for (i = 0; i < server_broadcast_ports_length; i++)
    {
        serv_addr.sin_port = htons(SERVER_BROADCAST_PORTS[i]);
        if (bind(server_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) >= 0)
        {
            Logger.write_info("Successfully bound to PORT " + std::to_string(SERVER_BROADCAST_PORTS[i]));
            break;
        }
        Logger.write_error("Failed binding to PORT " + std::to_string(SERVER_BROADCAST_PORTS[i]) + ". Will try next port...");
    }

    Logger.write_info("Server will start listening for broadcast");
    listen(server_socket, 5);

    while (true)
    {
        struct sockaddr_in cli_addr;
        char recvbuff[BROADCAST_MSG_LEN];
        int recvbufflen = BROADCAST_MSG_LEN;
        socklen_t len = sizeof(struct sockaddr_in);
        recvfrom(server_socket, recvbuff, recvbufflen, 0, (sockaddr *)&cli_addr, &len);

        Logger.write_debug("Received broadcast message");

        if (!isPrimaryServer)
            continue;

        int received_broadcast_msg = recvbuff[0];

        if (received_broadcast_msg != PRIMARY_BROADCAST_REQUEST)
        {
            std::cout << "Received unknown broadcast message. Ignored it" << std::endl;
            continue;
        }

        Logger.write_debug("Broadcast received request from client");

        std::string send_buffer = std::to_string(PRIMARY_BROADCAST_IAMPRIMARY_RESPONSE) + "\n" + get_local_ip() + "\n" + std::to_string(SELECTED_SERVER_PORT) + "\n";
        int send_buff_len = BROADCAST_MSG_LEN;

        socklen_t socklen_cli_addr = sizeof(cli_addr);
        if (sendto(server_socket, send_buffer.c_str(), BROADCAST_MSG_LEN + 1, 0, (sockaddr *)&cli_addr, socklen_cli_addr) < 0)
        {
            std::cout << "Error responding to broadcast" << errno << std::endl;
        }
    }
}

void *ServerCommunicationManager::updateClientsWithNewPrimaryServer(void *arg)
{
    for (std::pair<std::string, client_session> element : client_sessions)
    {
        client_session session = element.second;

        std::string receiver_ip = session.ip;
        std::string receiver_port = session.notification_port;
        std::string session_id = session.session_id;
        std::string message = get_local_ip() + "\n" + std::to_string(SELECTED_SERVER_PORT);

        int sockfd, n;
        struct sockaddr_in receiver_addr;
        struct hostent *receiver_host;
        struct in_addr addr;
        std::string buffer_out;

        inet_aton(receiver_ip.c_str(), &addr);

        receiver_host = gethostbyaddr(&addr, sizeof(receiver_ip), AF_INET);

        if (receiver_host == NULL)
        {
            Logger.write_error("[Send Notification] No such client found!");
        }

        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
            Logger.write_debug("[Send Notification] Opening socket");

        receiver_addr.sin_family = AF_INET;
        receiver_addr.sin_port = htons(std::stoi(receiver_port.c_str()));
        receiver_addr.sin_addr = *((struct in_addr *)receiver_host->h_addr);
        bzero(&(receiver_addr.sin_zero), 8);

        connect(sockfd, (struct sockaddr *)&receiver_addr, sizeof(receiver_addr));
        
        char *bufferPayload = (char *)calloc(MAX_DATA_SIZE, sizeof(char));
        packet packet_sent = {0, 0, 0, 0, session_id, bufferPayload};

        buildPacket(NOTIFICATION_NEW_PRIMARY_SERVER, 0, message, &packet_sent);

        if (Logger.get_log_level() == LEVEL_DEBUG)
        {
            packet_sent.print(std::string("SENT"));
        }

        char *buffer;
        buffer = (char *)calloc(MAX_MAIL_SIZE, sizeof(char));
        buffer = packet_sent.Serialize();

        n = write(sockfd, buffer, MAX_MAIL_SIZE);
        
        close(sockfd);
    }
}

std::unordered_map<std::string, client_session> ServerCommunicationManager::get_sessions()
{
    return client_sessions;
}

void ServerCommunicationManager::add_session(std::string cookie, std::string ip, std::string port)
{
    client_session new_session;
    
    new_session.ip = ip;
    new_session.notification_port = port;

    client_sessions[cookie] = new_session;
}

void ServerCommunicationManager::remove_session(std::string cookie)
{
    client_sessions.erase(cookie);
}

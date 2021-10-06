
#include "../../include/services/ClientCommunicationManager.h"
#include "../../include/services/Frontend.h"

using namespace socialine::utils;

std::string ClientCommunicationManager::username;
std::string ClientCommunicationManager::session_cookie;
int ClientCommunicationManager::notification_socket;
socklen_t ClientCommunicationManager::clilen;
models::manager::ClientNotificationManager* ClientCommunicationManager::notificationManager;

int ClientCommunicationManager::establish_connection(std::string username) {
    Frontend::discoverPrimaryServer();

    ClientCommunicationManager::username = username;
    ClientCommunicationManager::session_cookie = NO_COOKIE;

    char* bufferPayload = (char*) calloc(MAX_DATA_SIZE, sizeof(char));
    packet loginPacket = { 0,0,0,0, bufferPayload };

    std::string listen_notification_port;
    listen_notifications(&listen_notification_port);

    buildLoginPacket(username, listen_notification_port, &loginPacket);
    return sendPacket(&loginPacket);
}

int ClientCommunicationManager::sendPacket(struct __packet *packet) {
    int sockfd, n;
    struct sockaddr_in server_addr;
    struct hostent *server_host;
    struct in_addr addr;
    std::string buffer_out, buffer_in;

    inet_aton(Frontend::primaryServerIP.c_str(), &addr);
    server_host = gethostbyaddr(&addr, sizeof(Frontend::primaryServerIP), AF_INET);

    if (server_host == NULL) {
        std::cout << "No such host(" << Frontend::primaryServerIP << "): " << h_errno << std::endl;
        fprintf(stderr, "ERROR: %s\n", strerror(errno));
        fprintf(stderr, "ERROR: %s\n", strerror(h_errno));
        exit(0);
    }

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        Logger.write_error("Opening socket");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(Frontend::primaryServerPort);
    server_addr.sin_addr = *((struct in_addr *)server_host->h_addr);
    bzero(&(server_addr.sin_zero), 8);

    if (connect(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
        Logger.write_error("Connecting");

    packet->print("SENT");

    char* buffer;
    buffer = (char *) calloc(MAX_MAIL_SIZE, sizeof(char));
    buffer = packet->Serialize();

    // write
    n = write(sockfd, buffer, MAX_MAIL_SIZE);
    if (n < 0)
        Logger.write_error("Writing to socket");

    char buffer_response[MAX_MAIL_SIZE];
    bzero(buffer_response, MAX_MAIL_SIZE);

    n = read(sockfd, buffer_response, MAX_MAIL_SIZE);
    if (n < 0)
        Logger.write_error("Reading from socket");

    char received_packet_buffer[MAX_DATA_SIZE];
    struct __packet received_packet = {0, 0, 0, 0, received_packet_buffer };
    received_packet.Deserialize(buffer_response);
    received_packet.print("RECEIVED");

    if(session_cookie == NO_COOKIE)
        session_cookie = received_packet.cookie;

    close(sockfd);

    return received_packet.type;
}

int ClientCommunicationManager::follow(std::string followed)
{
    char* bufferPayload = (char*) calloc(MAX_DATA_SIZE, sizeof(char));
    packet pkt = { 0,0,0,0, bufferPayload };

    // _payload: follower + "\n" + followed + "\n"
    std::string pkt_msg = username + "\n" + followed + "\n";
    buildPacket(CMD_FOLLOW, 0, pkt_msg, &pkt);
    return sendPacket(&pkt);
}

int ClientCommunicationManager::tweet(std::string message) {
    char* bufferPayload = (char*) calloc(MAX_DATA_SIZE, sizeof(char));
    packet pkt = { 0,0,0,0, bufferPayload };

    std::string pkt_msg = username + "\n" + message + "\n";
    buildPacket(CMD_TWEET, 0, pkt_msg, &pkt);
    return sendPacket(&pkt);
}

int ClientCommunicationManager::logout() {
    char* bufferPayload = (char*) calloc(MAX_DATA_SIZE, sizeof(char));
    packet pkt = { 0,0,0,0, bufferPayload };

    std::string pkt_msg = username + "\n";
    buildPacket(CMD_LOGOUT, 0, pkt_msg, &pkt);
    return sendPacket(&pkt);
};

// Private Methods

void ClientCommunicationManager::buildLoginPacket(std::string username, std::string listen_notification_port, struct __packet *loginPacket) {
    std::string payload = username + "\n" + listen_notification_port + "\n";
    buildPacket(CMD_LOGIN, 0, payload, loginPacket);
}

void ClientCommunicationManager::buildPacket(uint16_t type, uint16_t seqn, std::string message, struct __packet *packet) {
    uint16_t length = HEADER_LENGTH + MAX_DATA_SIZE + COOKIE_LENGTH;
    uint16_t timestamp = getTimestamp();

    packet->type = type;
    packet->seqn = seqn;
    packet->length = length;
    packet->timestamp = timestamp;
    packet->cookie = session_cookie;
    packet->_payload = message;
}


uint16_t ClientCommunicationManager::getTimestamp() {
    time_t ti;
    ti = time(NULL);
    struct tm tm_time;
    tm_time = *localtime(&ti);

    //const time_t create_time;
    uint16_t d;
    d = tm_time.tm_mday
        + (tm_time.tm_mon + 1) * 32
        + (tm_time.tm_year - (1980-1900)) * 512;

    // Print ddmmyy
//    printf("%02d%02d%02d\n",
//           (int) d%32, (int) (d/32)%16, (int) ((d/512)%128 + (1980-1900))%100);
    return d;
}

void ClientCommunicationManager::listen_notifications(std::string *listen_notification_port) {

    // int notification_socket;
    // socklen_t clilen;
    struct sockaddr_in notf_addr;
    std::string input;

    if ((notification_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        Logger.write_error("Opening socket");

    notf_addr.sin_family = AF_INET;
    notf_addr.sin_addr.s_addr = INADDR_ANY;

    bzero(&(notf_addr.sin_zero), 8);

    if (bind(notification_socket, (struct sockaddr *) &notf_addr, sizeof(notf_addr)) < 0)
        Logger.write_error("[Notification Service] On binding");

    listen(notification_socket, 5);

    socklen_t len = sizeof(notf_addr);
    if (getsockname(notification_socket, (struct sockaddr *) &notf_addr, &len) == -1) {
        Logger.write_error("[Notification Service] On getsockname");
    } 
    else {
        *listen_notification_port = std::to_string(ntohs(notf_addr.sin_port));

        Logger.write_debug("[Notification Service] port number " + std::to_string(ntohs(notf_addr.sin_port)));
    }
    clilen = sizeof(struct sockaddr_in);

    Logger.write_debug("[Notification Service] Ready to receive");

    notificationManager = models::manager::ClientNotificationManager::get_instance();

    pthread_t ptid;
    pthread_create(&ptid, NULL, &thread_listen_notif, NULL);

    return;
}

void* ClientCommunicationManager::thread_listen_notif(void* arg)
{
    Logger.write_debug("Started notification listening service");

        int n;

        //Todo: change loop to detect the end of execution, so it can return to the app;
        while (true) {

            int connection_socket;
            struct sockaddr_in cli_addr;

            if ((connection_socket = accept(notification_socket, (struct sockaddr *) &cli_addr, &clilen)) == -1) {
                Logger.write_error("On accept");
                continue;
            }

            char buffer[MAX_MAIL_SIZE];
            bzero(buffer, MAX_MAIL_SIZE);
            n = read(connection_socket, buffer, MAX_MAIL_SIZE);
            if (n < 0)
                Logger.write_error("[Notification Service] Reading from socket");

            char received_packet_buffer[MAX_DATA_SIZE];
            struct __packet received_packet = {0, 0, 0, 0, received_packet_buffer };
            received_packet.Deserialize(buffer);
            received_packet.print("[Notification Service] RECEIVED");

            std::vector<std::string> fields = StringUtils::split(received_packet._payload, "\n");

            if (received_packet.type == NOTIFICATION_NEW_PRIMARY_SERVER) {
                std::string newPrimaryIP = fields[0];
                std::string newPrimaryPort = fields[1];
                Frontend::updatePrimaryServer(newPrimaryIP, stoi(newPrimaryPort));

            } else {

                notificationManager->receive_notification(
                        fields[0],
                        static_cast<uint32_t>(std::stoul(fields[1])),
                        fields[2]
                );
            }
            
        }

        close(notification_socket);
}
#include <thread>
#include "../include/RingLeaderElection.hpp"
#include "../../Utils/Scheduler.hpp"
#include "../../Utils/Logger.h"
#include "../../Utils/StringUtils.h"

#define MSG_ELECTION 5
#define MSG_ELECTED 6
#define TIMEOUT_RECEIVE_ELECTION_MS 10000

using namespace socialine::util::task;
using namespace socialine::utils;

//-------------------------------------------------------------------------
//		Constructor
//-------------------------------------------------------------------------
RingLeaderElection::RingLeaderElection(std::vector<Server>* replic_managers)
{
    this->replic_managers = replic_managers;
    leader_elected = false;
}


//-------------------------------------------------------------------------
//		Methods
//-------------------------------------------------------------------------
bool RingLeaderElection::start_election_leader(Server starter)
{
    std::map<int, std::vector<Server>> servers_mapped_by_pid = get_servers_ordered_by_pid_ascending();
    bool is_leader = false;
    bool participant = false;
    int leader_pid = -1;
    int idx = get_index_of(starter);
    int next_idx = (idx+1) % replic_managers->size();
    int my_pid = starter.get_pid();
    std::thread receiver(&RingLeaderElection::receive_election_leader, this, starter);

    receiver.detach();

    /* First round */
    // Tries to receive a message from the neighbour of the starter. If no
    // message is received, it means that starter is the first to start
    // leader election.
    sleep(2);
    
    if (!did_neighbor_send_something())
        candidate_pid = -1;

    if (candidate_pid < my_pid)
    {
        leader_pid = candidate_pid;
        send_election_leader(MSG_ELECTION, my_pid, (*replic_managers)[next_idx]);
    }
    else if (!participant)
    {
        participant = true;
        send_election_leader(MSG_ELECTION, candidate_pid, (*replic_managers)[next_idx]);
    }
    else if (candidate_pid == my_pid)
    {
        send_election_leader(MSG_ELECTED, my_pid, (*replic_managers)[next_idx]);
    }

    /* Second round */
    wait_leader_be_elected();
    sleep(5);   // Waits 5 seconds in order to all servers that belongs to the 
                // ring receive a message.

    if (my_pid == leader_pid)
    {
        is_leader = true;
        std::cout << "I, " << my_pid << ", AM THE LEADER :D ! hahahaha" << std::endl;
    }
    else
    {
        std::cout << "I, " << my_pid << ", AM BACKUP AGAIN ;(" << std::endl;
    }

    return is_leader;
}

int RingLeaderElection::get_index_of(Server server)
{
    int idx = 0;

    for (auto it = replic_managers->begin(); it != replic_managers->end(); it++)
    {
        if (it->get_signature() == server.get_signature())
            return idx;

        idx++;
    }

    return -1;
}

void RingLeaderElection::wait_leader_be_elected()
{
    while (!leader_elected)
        sleep(1);
}

std::map<int, std::vector<Server>> RingLeaderElection::get_servers_ordered_by_pid_ascending()
{
    std::map<int, std::vector<Server>> servers;

    for (Server server : *replic_managers)
    {
        if (servers.find(server.get_pid()) == servers.end())
        {
            std::vector<Server> servers_with_same_pid;
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

Server RingLeaderElection::get_server_with_pid(int pid)
{
    for (Server s : *replic_managers) 
    {
        if (s.get_pid() == pid)
            return s;
    }

    return Server("", 0, 0);
}


void RingLeaderElection::receive_election_leader(Server receiver)
{
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

    while (!leader_elected)
    {
        connection_socket = accept(server_socket, (struct sockaddr *)&cli_addr, &clilen);

        if (connection_socket == -1)
            continue;

        read_bytes_from_socket = read(connection_socket, buffer_response, MAX_MAIL_SIZE);

        if (read_bytes_from_socket < 0)
        {
            fprintf(stderr, "socket() failed: %s\n", strerror(errno));
            Logger.write_error("ERROR: Reading from socket");
            close(connection_socket);
            exit(-1);
        }

        if (buffer_response[0] == MSG_ELECTION)
        {
            candidate_pid = ntohl(
                buffer_response[1] << 24
                | buffer_response[2] << 16
                | buffer_response[3] << 8
                | buffer_response[4]
            );
        }
        else if (buffer_response[0] == MSG_ELECTED)
        {
            leader_elected = ntohl(
                buffer_response[1] << 24
                | buffer_response[2] << 16
                | buffer_response[3] << 8
                | buffer_response[4]
            );

            leader_elected = true;
        }

        close(connection_socket);
    }
  
    close(connection_socket);
    close(server_socket);
}

bool RingLeaderElection::did_neighbor_send_something()
{
    return (candidate_pid > 0);
}

void RingLeaderElection::send_election_leader(int message_type, int pid, Server to)
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
    uint32_t noreplic_managersalizedPid = htonl(pid);

    message[0] = message_type;

    message[1] = noreplic_managersalizedPid >> 24;
    message[2] = noreplic_managersalizedPid >> 16;
    message[3] = noreplic_managersalizedPid >> 8;
    message[4] = noreplic_managersalizedPid;

    n = write(sockfd, message, MAX_MAIL_SIZE);
    if (n < 0)
        Logger.write_error("Failed to write to socket");
}

in_addr RingLeaderElection::get_ip_by_address(std::string address)
{
    hostent *server_host;
    struct in_addr addr;
    inet_aton(address.c_str(), &addr);
    server_host = gethostbyaddr(&addr, sizeof(address), AF_INET);

    return *((struct in_addr *)server_host->h_addr);
}


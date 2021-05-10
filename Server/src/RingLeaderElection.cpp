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
    candidate_pid = 0;
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
    unsigned int my_pid = starter.get_pid();

    std::thread receiver(&RingLeaderElection::receive_election_leader, this, starter);
    receiver.detach();
    sleep(2);
    
    while (address_already_in_use)
        sleep(2);

    /* First round */
    // Tries to receive a message from the neighbour of the starter. If no
    // message is received, it means that starter is the first to start
    // leader election.
    
    while (!leader_elected)
    {
        if (!did_neighbor_send_something())
        {
            candidate_pid = 0;
            std::cout << starter.get_signature() << " " << starter.get_pid() << " - I'M STARTING ELECTION LEADER" << std::endl;
        }

        if (candidate_pid < my_pid)
        {
            if (candidate_pid > 0)
                std::cout << starter.get_signature() << " " << starter.get_pid() << " - MY PID IS GREATHER THAN THE PID RECEIVED" << std::endl;

            leader_pid = candidate_pid;
            send_election_leader(MSG_ELECTION, my_pid, (*replic_managers)[next_idx]);
        }
        else if (!participant && candidate_pid != my_pid)
        {
            std::cout << starter.get_signature() << " " << starter.get_pid() << " - MY PID IS LESS THAN THE PID RECEIVED" << std::endl;
            std::cout << starter.get_signature() << " " << starter.get_pid() << " - SENDING MSG_ELECTION " << candidate_pid << " TO " << (*replic_managers)[next_idx] << std::endl;
            participant = true;
            send_election_leader(MSG_ELECTION, candidate_pid, (*replic_managers)[next_idx]);
        }
        else if (candidate_pid == my_pid)
        {
            std::cout << starter.get_signature() << " " << starter.get_pid() << " - SENDING MSG_ELECTED " << candidate_pid << " TO " << (*replic_managers)[next_idx] << std::endl;
            send_election_leader(MSG_ELECTED, my_pid, (*replic_managers)[next_idx]);
            leader_elected = true;
            leader_pid = my_pid;
        }
    }

    /* Second round */
    std::cout << starter.get_signature() << " " << starter.get_pid() << "WAITING SECOND ROUND" << std::endl;
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
    char buffer_response[MAX_MAIL_SIZE];
    struct sockaddr_in serv_addr;
    struct in_addr addr;
    hostent *server_host;
    address_already_in_use = true;

    std::cout << receiver.get_signature() << " " << receiver.get_pid() << " - RECEIVE - ELECTION LEADER" << std::endl;

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

    //sleep(4); // Avoids 'Address already in use' error
    int option = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) < 0)
    {
        fprintf(stderr, "%s\n", strerror(errno));
        exit(-1);
    }

    while (address_already_in_use)
    {
        if (bind(server_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) >= 0)
            address_already_in_use = false;
    }
    std::cout << receiver.get_signature() << " " << receiver.get_pid() << " - RECEIVE - OK" << std::endl;

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

            std::cout << receiver.get_signature() << " " << receiver.get_pid() << " - RECEIVED MSG_ELECTION " << candidate_pid << std::endl;
        }
        else if (buffer_response[0] == MSG_ELECTED)
        {
            leader_pid = ntohl(
                buffer_response[1] << 24
                | buffer_response[2] << 16
                | buffer_response[3] << 8
                | buffer_response[4]
            );

            leader_elected = true;

            std::cout << receiver.get_signature() << " " << receiver.get_pid() << " - RECEIVED MSG_ELECTED " << leader_pid << std::endl;
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

void RingLeaderElection::send_election_leader(int message_type, unsigned int pid, Server to)
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

    sleep(2);
    if (connect(sockfd, (struct sockaddr *)&backup_server_addr, sizeof(backup_server_addr)) < 0)
    {
        fprintf(stderr, "socket() failed - SEND: %s\n", strerror(errno));
        exit(-1); 
    }

    char *message = new char[MAX_MAIL_SIZE];
    uint32_t normalized_pid = htonl(pid);

    message[0] = message_type;

    message[1] = normalized_pid >> 24;
    message[2] = normalized_pid >> 16;
    message[3] = normalized_pid >> 8;
    message[4] = normalized_pid;

    n = write(sockfd, message, MAX_MAIL_SIZE);
    if (n < 0)
        Logger.write_error("Failed to write to socket");

    close(sockfd);
}

in_addr RingLeaderElection::get_ip_by_address(std::string address)
{
    hostent *server_host;
    struct in_addr addr;
    inet_aton(address.c_str(), &addr);
    server_host = gethostbyaddr(&addr, sizeof(address), AF_INET);

    return *((struct in_addr *)server_host->h_addr);
}


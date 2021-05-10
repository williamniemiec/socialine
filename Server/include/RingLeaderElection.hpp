#pragma once
#include <map>
#include <vector>
#include <netdb.h>
#include "Server.hpp"
#include "../../Utils/Types.h"

/// <summary>
///     Responsible for conducting leader election using the Ring Algorithm.
/// </summary>
class RingLeaderElection
{
//-------------------------------------------------------------------------
//		Attributes
//-------------------------------------------------------------------------
private:
    int leader_pid;
    int candidate_pid;
    bool leader_elected;
    std::vector<Server>* replic_managers;
    std::map<int, std::vector<Server>>* servers_mapped_by_pid;
    socklen_t clilen;
    int server_socket;
    int connection_socket;
    struct sockaddr_in cli_addr;
    int read_bytes_from_socket;
    char buffer_response[MAX_MAIL_SIZE];


//-------------------------------------------------------------------------
//		Constructor
//-------------------------------------------------------------------------
public:
    RingLeaderElection(std::vector<Server>* replic_managers);


//-------------------------------------------------------------------------
//		Methods
//-------------------------------------------------------------------------
public:
    bool start_election_leader(Server starter);
private:
    in_addr get_ip_by_address(std::string address);
    std::map<int, std::vector<Server>> get_servers_ordered_by_pid_ascending();
    Server get_server_with_pid(int pid);
    void receive_election_leader(Server receiver);
    void send_election_leader(int message_type, int pid, Server to);
    int get_index_of(Server server);
    void wait_leader_be_elected();
    bool did_neighbor_send_something();
};

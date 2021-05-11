#pragma once
#include <vector>
#include <map>
#include <netinet/in.h>
#include <unordered_map>
#include "../../Utils/Types.h"
#include "../../Utils/IObservable.hpp"
#include "../../Utils/IObserver.hpp"
#include "Server.hpp"

/// <summary>
///     Responsible for managing primary server and backup servers.
/// </summary
class ReplicManager : public IObserver, public IObservable
{
//-------------------------------------------------------------------------
//		Attributes
//-------------------------------------------------------------------------
private:
    std::list<IObserver*> observers;
    std::vector<Server>* rm; // Replic managers
    bool is_primary;
    socklen_t clilen;
    int server_socket;
    int connection_socket;
    struct sockaddr_in cli_addr;
    int readBytesFromSocket;
    char buffer_response[MAX_MAIL_SIZE];
    std::string primaryIp;
    std::string multicastIp;
    unsigned int myPid;


//-------------------------------------------------------------------------
//		Constructor
//-------------------------------------------------------------------------
public:
    ReplicManager();


//-------------------------------------------------------------------------
//		Methods
//-------------------------------------------------------------------------
public:
    /// <summary>
    ///     Initializes a server as a primary if there is no server running,
    ///     or as a backup if one exists.
    /// </summary>
    void run();

    virtual void attach(IObserver* observer);
    virtual void detatch(IObserver* observer);
    virtual void notify_observers();
    virtual void update(IObservable* observable, std::list<std::string> data);
    void notify_pending_notification(std::string followed, notification current_notification);
    void send_all_sessions(Server target, std::unordered_map<std::string, client_session> sessions);
    void send_all_followers(Server target, std::unordered_map<std::string, std::vector<std::string>> followers);
    void send_all_profile_sessions(Server target, std::unordered_map<std::string, std::vector<std::string>> sessions);
    void notify_list_backups(std::vector<Server>* backups);
    void notify_primary_addr();
private:
    void notify_new_follow(std::string follower, std::string followed);
    void notify_new_session(std::string sessionId, client_session client_session);
    void notify_new_profile_session(std::string username, std::string sessionId);
    void notify_close_session(std::string sessionId);
    void notify_close_profile_session(std::string username);
    void multicast_signal();
    bool try_receive_multicast_signal();
    bool is_primary_active();
    void init_server_as_primary();
    std::string get_local_ip();
    void heartbeat_sender();
    void heartbeat_receiver();
    void add_new_backup_server(std::string ip, uint16_t port, pid_t pid);
    void config_new_backup_server(int connection_socket, sockaddr_in cli_addr);
    void new_backup_service();
    void receive_primary_addr();
    in_addr get_ip_by_address(std::string address);
    uint16_t get_available_port_in_range(uint16_t start, uint16_t end);
    bool is_port_available(uint16_t port);
    void connect_with_primary_server(std::string backupIp, uint16_t backupPort);
    void init_server_as_backup();
    void send_follow(Server server, std::string follower, std::string followed);
    void send_session(Server server, std::string sessionId, client_session client_session);
    void send_pending_notification(Server server, std::string followed, notification current_notification);
    void send_all_pending_notifications(Server target, std::unordered_map<std::string, std::vector<notification>> notifications);
    void send_close_pending_notification(Server server, std::string username);
    void notify_close_pending_notification(std::string username);
    void send_all_sessions(std::unordered_map<std::string, client_session>, Server target);
    //void send_all_followers(Server target);
    void send_profile_session(Server server, std::string username, std::string session_id);
    void send_profile_follow(Server server, std::string username, std::string followed);
    void send_all_pending_notifications(Server target);
};

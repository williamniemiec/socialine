#include <list>
#include <netinet/in.h>
#include <unordered_map>
#include "../../Utils/Types.h"
#include "Server.hpp"

/// <summary>
///     Responsible for managing primary server and backup servers.
/// </summary
class ReplicManager
{
//-------------------------------------------------------------------------
//		Attributes
//-------------------------------------------------------------------------
public:
    //static int g_availablePort;
    static int g_process_id;
    static std::list<Server>* rm; // Replic managers
    static bool g_primary_server_online;
    static socklen_t clilen;
    static int server_socket;
    static int connection_socket;
    static struct sockaddr_in cli_addr;
    static int readBytesFromSocket;
    static char buffer_response[MAX_MAIL_SIZE];
    static std::string primaryIp;
    static std::string multicastIp;


//-------------------------------------------------------------------------
//		Constructor
//-------------------------------------------------------------------------
private:
    ReplicManager();


//-------------------------------------------------------------------------
//		Methods
//-------------------------------------------------------------------------
public:
    /// <summary>
    ///     Initializes a server as a primary if there is no server running,
    ///     or as a backup if one exists.
    /// </summary>
    static void run();
private:
    static void multicast_signal();
    static bool try_receive_multicast_signal();
    static bool is_primary_active();
    static void init_server_as_primary();
    static std::string get_local_ip();
    static void heartbeat_sender();
    static void heartbeat_receiver();
    static void add_new_backup_server(std::string ip, uint16_t port);
    static void config_new_backup_server(int connection_socket, sockaddr_in cli_addr);
    static void service_new_backup();
    static void notify_pending_notification(std::string followed, notification current_notification);
    static void notify_new_session(client_session client_session);
    static void notify_close_session(client_session client_session);
    static void notify_follow(std::string follower, std::string followed);
    static void notify_list_backups(std::list<Server>* backups);
    static void notify_primary_addr();
    static void receive_primary_addr();
    static in_addr get_ip_by_address(std::string address);
    static uint16_t get_available_port_in_range(uint16_t start, uint16_t end);
    static bool is_port_available(uint16_t port);
    static void connect_with_primary_server(std::string backupIp, uint16_t backupPort);
    static void init_server_as_backup();
};

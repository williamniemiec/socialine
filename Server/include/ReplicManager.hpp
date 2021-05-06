#include <map>
#include <netinet/in.h>
#include <unordered_map>
#include "../../Utils/Types.h"

/// <summary>
///     Responsible for managing primary server and backup servers.
/// </summary
class ReplicManager
{
//-------------------------------------------------------------------------
//		Attributes
//-------------------------------------------------------------------------
public:
    static int g_availablePort;
    static int g_process_id;
    static std::map<int, sockaddr_in>* rm; // Replic managers
    static bool g_primary_server_online;
    static socklen_t clilen;
    static int server_socket;
    static int connection_socket;
    static struct sockaddr_in cli_addr;
    static int readBytesFromSocket;
    static char buffer_response[MAX_MAIL_SIZE];


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
    static bool is_primary_active();
    static void init_server_as_primary();
    static void heartbeat_sender();
    static void heartbeat_receiver();
    static void add_new_backup_server(sockaddr_in cli_addr);
    static void config_new_backup_server(int connection_socket, sockaddr_in cli_addr);
    static void service_new_backup();
    static void notify_pending_notification(std::string followed, notification current_notification);
    static void notify_new_session(client_session client_session);
    static void notify_close_session(client_session client_session);
    static void notify_follow(std::string follower, std::string followed);
    static void notify_list_backups(std::map<int, sockaddr_in>* backups);
    static void notify_new_backup();
    static uint16_t ask_primary_available_port();
    static void init_server_as_backup();
};

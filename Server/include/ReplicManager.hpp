#include <map>
#include <netinet/in.h>
#include "../../Utils/Types.h"

/// <summary>
///     Responsible for executing routines within time intervals.
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
    ///     Runs a routine within a timeout.
    /// </summary>
    static void run();
private:
    static bool is_primary_active();
    static void heartbeat_sender();
    static void heartbeat_receiver();
    static void add_new_backup_server(sockaddr_in cli_addr);
    static void config_new_backup_server(int connection_socket, sockaddr_in cli_addr);
    static void service_new_backup();
    static uint16_t ask_primary_available_port();
    static void init_server_as_backup();
};

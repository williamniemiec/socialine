#pragma once
#include <unordered_map>
#include <vector>
#include "../../Utils/Types.h"

/// <summary>
///     Responsible for managing server data.
/// </summary>
class DataManager
{
//-------------------------------------------------------------------------
//		Attributes
//-------------------------------------------------------------------------
private:
    /// Key: cookie; Value: client_session
    static std::unordered_map<std::string, client_session> sessions;

    // Key: user; Value: List of followers
    static std::unordered_map<std::string, std::vector<std::string>> followers;

    // Key: user; Value: List of users followed by the user
    static std::unordered_map<std::string, std::vector<std::string>> followedBy;

    // Key: user; Value: List of pending notifications
    static std::unordered_map<std::string, std::vector<notification>> pendingNotifications;


//-------------------------------------------------------------------------
//		Constructor
//-------------------------------------------------------------------------
private:
    DataManager();


//-------------------------------------------------------------------------
//		Methods
//-------------------------------------------------------------------------
public:
    static void new_session(std::string sessionId, client_session session);
    static void remove_session(std::string sessionId);
    static bool follow(std::string follower, std::string followed);
    static void new_pending_notification(std::string user, notification pendingNotification);
    static std::vector<notification> read_pending_notification(std::string username);
    static std::unordered_map<std::string, client_session> get_all_sessions();
    static std::unordered_map<std::string, std::vector<std::string>> get_all_followers();
    static std::unordered_map<std::string, std::vector<notification>> get_all_pending_notifications();
};

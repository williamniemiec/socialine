//
// Created by Farias, Karine on 3/20/21.
//

#ifndef CLIENT_APP_TYPES_H
#define CLIENT_APP_TYPES_H
#endif //CLIENT_APP_TYPES_H

#include <cstdint>
#include <vector>
#include <string>

#define MAX_ARGS 10
#define NEW_LINE '\n'

#define CMD_LOGIN 1
#define CMD_FOLLOW 2
#define CMD_OTHERS 3

#define ERROR_INVALID_COMMAND 1
#define ERROR_START_WITH_LOGIN 2
#define ERROR_SESSION_ALREADY_STARTED 3
#define ERROR_SERVER_DOESNT_EXIST 5
#define ERROR_SESSIONS_LIMIT 7
#define ERROR_FOLLOW_YOURSELF 11
#define ERROR_OTHERS 13

#define MESSAGE_SHUT_DOWN 21

typedef struct __command{
    int type;
    std::vector<std::string> arguments;
}app_command;

typedef struct __packet{
    uint16_t type;          //PACKAGE TYPE (Eg. data/command)
    uint16_t seqn;          //Sequence number
    uint16_t length;        //Payload size
    uint16_t timestamp;     //Data timestamp
    const char* _payload;   //Message data
} packet;

typedef struct __notification{
    uint32_t id;            //Notification ID
    uint32_t timestamp;     //Notification timestamp
    const char* _message;   //Message data
    uint16_t length;        //Message size
    uint16_t pending;       //Number of pending readers
} notification;
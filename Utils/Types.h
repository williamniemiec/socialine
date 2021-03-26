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
#define CMD_LOGOUT 3
#define CMD_OTHERS 5

#define ERROR_INVALID_COMMAND 1
#define ERROR_START_WITH_LOGIN 2
#define ERROR_SESSION_ALREADY_STARTED 3
#define ERROR_SERVER_DOESNT_EXIST 5
#define ERROR_SESSIONS_LIMIT 7
#define ERROR_FOLLOW_YOURSELF 11
#define ERROR_OTHERS 13

#define MESSAGE_SHUT_DOWN 21

#define MAX_MAIL_SIZE 1024
#define MAX_DATA_SIZE 256

typedef struct __command{
    int type;
    std::vector<std::string> arguments;
}app_command;

typedef struct __packet{
    uint16_t type;          //PACKAGE TYPE (Eg. data/command)
    uint16_t seqn;          //Sequence number
    uint16_t length;        //Payload size
    uint16_t timestamp;     //Data timestamp
    std::string _payload;   //Message data

    char* Serialize() {
        char *message = new char [MAX_MAIL_SIZE];
        message[0] = htons(type) >> 8;
        message[1] = htons(type);

        message[2] = htons(seqn) >> 8;
        message[3] = htons(seqn);

        message[4] = htons(length) >> 8;
        message[5] = htons(length);

        message[6] = htons(timestamp) >> 8;
        message[7] = htons(timestamp);

        memcpy(&message[8], _payload.c_str(), MAX_DATA_SIZE);

        return message;
    }

    void Deserialize(char *message) {

        type = ntohs(message[0] << 8 | message[1]);
        seqn = ntohs(message[2] << 8 | message[3]);
        length = ntohs(message[4] << 8 | message[5]);
        timestamp = ntohs(message[6] << 8 | message[7]);

        char payload_message[MAX_DATA_SIZE];
        for (unsigned i=0; i < MAX_DATA_SIZE; i++)
            payload_message[i] = message[i+8];

        _payload.assign(payload_message, MAX_DATA_SIZE);
    }

    void print(char* title) {
        printf("---- %s PACKET -----\n", title);
        printf("packet.type: %u\n",this->type);
        printf("packet.seqn: %u\n", this->seqn);
        printf("packet.length: %u\n", this->length);
        printf("packet.timestamp: %u\n", this->timestamp);
        printf("packet._payload: %s\n", this->_payload.c_str());
        printf("----------------------\n");

    }


} packet;

typedef struct __notification{
    uint32_t id;            //Notification ID
    uint32_t timestamp;     //Notification timestamp
    const char* _message;   //Message data
    uint16_t length;        //Message size
    uint16_t pending;       //Number of pending readers
} notification;
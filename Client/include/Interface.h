//
// Created by Farias, Karine on 3/17/21.
//

#ifndef SOCIALINEV2_INTERFACE_H
#define SOCIALINEV2_INTERFACE_H

#include "../../Utils/Types.h"
#include <iostream>

class Interface {
    public:
        app_command parse_command(int argc, char* argv[]);
        app_command parse_command(std::string);
        void print_message(int message_code);

};


#endif //SOCIALINEV2_INTERFACE_H

//
// Created by Farias, Karine on 4/25/21.
//

#ifndef SOCIALINE_LOGGER_H
#define SOCIALINE_LOGGER_H

#include <string>

namespace socialine::utils
{
    class SocialineLogger {
        int log_level;
    public:
        void set_log_level( int level );
        void write_debug(std::string message);
        void write_error(std::string message);
        void write_info(std::string message);
        int  get_log_level(  );

        //possible useful extensions: dump file and margin level
    };


    static SocialineLogger Logger;
}


#endif //SOCIALINE_LOGGER_H

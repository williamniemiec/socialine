//
// Created by Farias, Karine on 3/17/21.
//

#ifndef SOCIALINEV2_PROFILESESSIONMANAGER_H
#define SOCIALINEV2_PROFILESESSIONMANAGER_H

#include <iostream>

class ProfileSessionManager {

    int login( std::string username );
    int follow( std::string follower, std::string followed );

};


#endif //SOCIALINEV2_PROFILESESSIONMANAGER_H

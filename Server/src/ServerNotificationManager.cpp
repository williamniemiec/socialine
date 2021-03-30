//
// Created by Farias, Karine on 3/17/21.
//

#include "../include/ServerNotificationManager.h"

void ServerNotificationManager::tweet( )
{
    tweet_number++;
}

int ServerNotificationManager::read_tweet( )
{
    return tweet_number;
}
//
// Created by Farias, Karine on 3/17/21.
//

#ifndef SOCIALINEV2_CLIENTCOMMUNICATIONMANAGER_H
#define SOCIALINEV2_CLIENTCOMMUNICATIONMANAGER_H


class ClientCommunicationManager {
    public:
        int establish_connection(std::string username, string server , string door );
        int follow(string followed);

};


#endif //SOCIALINEV2_CLIENTCOMMUNICATIONMANAGER_H

#include <iostream>
#include "client/Client.h"

using namespace std;

int main(int argc, char* args[])
{
    //string perfil = args[1];
    //string server = args[2];
    //string porta = args[3];
    string perfil = "fulano123";
    string server = "123.456.789.000";
    string porta = "1234";

    client::Client* client = new client::Client(perfil, server, porta);
    client->render();

    return 0;
}

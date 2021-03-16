#include <iostream>
#include "client/Client.hpp"

using namespace std;

int main(int argc, char* args[])
{
    //string perfil = args[1];
    //string server = args[2];
    //string porta = args[3];
    string perfil = "fulano123";
    string server = "123.456.789.000";
    string porta = "1234";

    client::Client::login(perfil, server, porta);

    return 0;
}

#include "../../include/services/Frontend.h"

std::string Frontend::primaryServerIP;
uint16_t Frontend::primaryServerPort;
std::string Frontend::broadcastIP = ""; //TODO: definir um IP de broadcast para ser usado no IP Multicast

void Frontend::discoverPrimaryServer() {
    // Envia mensagem para um grupo através de IP Multicast. É esperado que o primary server responda
    // dizendo basicamente "eu sou o servidor primário".

    // do lado dos servidores, o IP deste cliente será adicionado na lista de clientes abertos.
    // essa lista é importante pra caso o primary server caia, os clientes abertos recebam o IP do novo
    // primary server.

    // TODO: remove hard coded values
    primaryServerIP = "127.0.0.1";
    primaryServerPort = 4000;
}

// Este método deve ser chamado apenas quando receber um novo IP de servidor primário
void Frontend::updatePrimaryServer(std::string newIP, int newPort) {
    // O communicationManager já está com porta aberta para receber notificações do servidor. Por essa
    // mesma porta ele irá receber também o novo IP do servidor primário, caso troque. Ao receber esse
    // novo IP, irá chamar essa função para atualizar o primaryServerIP;

    primaryServerIP = newIP;
    primaryServerPort = newPort;
}
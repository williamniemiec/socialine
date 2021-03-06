#include "../../include/services/Frontend.h"

std::string Frontend::primaryServerIP;
uint16_t Frontend::primaryServerPort;

using namespace socialine::utils;

int SERVER_BROADCAST_PORTS[] = { 4010, 4011, 4012, 4013, 4014, 4015, 4016, 4017, 4018, 4019, 4020 };

void Frontend::discoverPrimaryServer() {
    // Envia mensagem para um grupo através de IP Multicast. É esperado que o primary server responda
    // dizendo basicamente "eu sou o servidor primário".

    // do lado dos servidores, o IP deste cliente será adicionado na lista de clientes abertos.
    // essa lista é importante pra caso o primary server caia, os clientes abertos recebam o IP do novo
    // primary server.


    int sock;
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    int broadcast = 1;
    socklen_t sizeof_broadcast = sizeof(broadcast);
    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof_broadcast) < 0) {
        std::cout << "Error in setting up broadcast: " << errno << std::endl;
        close(sock);
        exit(0);
    }

    struct timeval tv;
    tv.tv_sec = 2;
    tv.tv_usec = 0;
    if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv) < 0) {
        std::cout << "Error in setting up broadcast receive timeout: " << errno << std::endl;
        close(sock);
        exit(0);
    };

    struct sockaddr_in recv_addr;
    struct sockaddr_in sender_addr;

    int len = sizeof(struct sockaddr_in);

    char sendMSG[] = "";
    sendMSG[0] = PRIMARY_BROADCAST_REQUEST;

    char recvbuff[BROADCAST_MSG_LEN] = "";
    char recvbufflen = BROADCAST_MSG_LEN;

    recv_addr.sin_family = AF_INET;
    recv_addr.sin_addr.s_addr = INADDR_BROADCAST;


    int i = 0;
    int server_broadcast_ports_length = sizeof(SERVER_BROADCAST_PORTS)/sizeof(SERVER_BROADCAST_PORTS[0]);
    do {
        std::cout << "Broadcasting to PORT " << std::to_string(SERVER_BROADCAST_PORTS[i]) << "." << std::endl;

        recv_addr.sin_port = htons(SERVER_BROADCAST_PORTS[i]); //MANDAR BROADCAST PRA CADA UMA DAS PORTAS DE SERVER_BROADCAST_PORTS.

        socklen_t sizeof_recv_addr = sizeof(recv_addr);
        sendto(sock, sendMSG, strlen(sendMSG) + 1, 0, (sockaddr *) &recv_addr, sizeof_recv_addr);

        socklen_t socklen_len = len;
        recvfrom(sock, recvbuff, recvbufflen, 0, (sockaddr *) &recv_addr, &socklen_len);

        i = (i+1) % server_broadcast_ports_length;

    } while (strlen(recvbuff) == 0);

    std::cout << "\n\n Received answer to broadcast sent: " << recvbuff << std::endl;

    std::vector<std::string> message = StringUtils::split(recvbuff,"\n");

    primaryServerIP = message[1];//"127.0.0.1";//inet_ntoa(recv_addr.sin_addr);
    primaryServerPort = stoi(message[2]); //htons(recv_addr.sin_port);

    std::cout << "Primary Server IP and PORT: " << primaryServerIP << ":" << primaryServerPort << std::endl;

    close(sock);
}

// Este método deve ser chamado apenas quando receber um novo IP de servidor primário
void Frontend::updatePrimaryServer(std::string newIP, int newPort) {
    // O communicationManager já está com porta aberta para receber notificações do servidor. Por essa
    // mesma porta ele irá receber também o novo IP do servidor primário, caso troque. Ao receber esse
    // novo IP, irá chamar essa função para atualizar o primaryServerIP;

    primaryServerIP = newIP;
    primaryServerPort = newPort;

    std::cout << "\n\n Updated Primary Server to " << primaryServerIP << ":" << primaryServerPort << std::endl;

}

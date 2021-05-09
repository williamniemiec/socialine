#include "../include/Server.hpp"

Server::Server(std::string ip, uint16_t port)
{
    this->ip = ip;
    this->port = port;
}

std::string Server::get_ip()
{
    return this->ip;
}

uint16_t Server::get_port()
{
    return this->port;
}

std::string Server::get_signature()
{
    return this->ip + ":" + std::to_string(port);
}

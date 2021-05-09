#include "../include/Server.hpp"

Server::Server(std::string ip, uint16_t port, int pid)
{
    this->ip = ip;
    this->port = port;
    this->pid = pid;
}

std::string Server::get_ip()
{
    return this->ip;
}

uint16_t Server::get_port()
{
    return this->port;
}

int Server::get_pid()
{
    return this->pid;
}

std::string Server::get_signature()
{
    return this->ip + ":" + std::to_string(port);
}

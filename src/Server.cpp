#include "Server.hpp"


Server::Server(void)
{
    return;
}

Server::Server(Server const &src)
{
   *this = src; 
}

Server::~Server(void)
{
    return;
}

Server &Server::operator=(Server const &rhs)
{
    *this = rhs; 
    return *this;
}


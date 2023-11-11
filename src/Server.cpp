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

Server& Server::operator=(const Server& rhs)
{
    if (this != &rhs) // Check for self-assignment
    { 
        // Copy each member from 'rhs' to 'this' and copy other members as needed
        _directives = rhs._directives;
        _locations = rhs._locations;       
    }
    return *this;
}


void Server::setDirectives(const std::string &name, const std::string &value)
{
    _directives[name] = value;
}






/* DEBUG */

void Server::printDirectives(void) const
{
    for (std::map<std::string, std::string>::const_iterator it = _directives.begin(); it != _directives.end(); ++it)
    {
        std::cout << "  " << it->first << ": " << it->second << std::endl;
    }
}

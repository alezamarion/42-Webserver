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
    if (this != &rhs) // Check for self-assignment avoiding stack overflow
    { 
        *this = rhs;       
    }
    return *this;
}

// void Server::setDirectives(const std::string &name, const std::string &value)
// {

//     if (name == "listen")
//     {
//         // Convert directiveValue to the appropriate type, e.g., int
//         // and set the corresponding member variable
//     } 
//     else if (name == "server_name")
//     {
//         // Set the server_name member variable
//     }
//     else if (name == "root")
//     {
//         // Set the root member variable
//     }




// }

/* DEBUG */

// void Server::printDirectives(void) const
// {
//     for (std::map<std::string, std::string>::const_iterator it = _directives.begin(); it != _directives.end(); ++it)
//     {
//         std::cout << "  " << it->first << ": " << it->second << std::endl;
//     }
// }

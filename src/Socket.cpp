#include "Socket.hpp"
#include "ConfigParser.hpp"

Socket::Socket(void)
{
    return;
}


Socket::Socket(Socket const &src)
{
   *this = src; 
}

Socket::~Socket(void)
{
    return;
}

Socket& Socket::operator=(const Socket& rhs)
{
    // Check for self-assignment avoiding stack overflow
    if (this != &rhs) 
    { 
        *this = rhs;
    }
    return *this;
}

void Socket::initSockets(ConfigParser& configParser)
{
    const std::vector<ConfigSpec>& configSpecs = configParser.getConfigSpecs();

    // Iterate through each ConfigSpec and initialize sockets based on the configuration
    std::vector<ConfigSpec>::const_iterator it;
    for (it = configSpecs.begin(); it != configSpecs.end(); ++it)
    {
        // Use the configuration in *it to initialize sockets
        // ...
    }
}


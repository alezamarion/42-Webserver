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

void Server::setDirectives(const std::map<std::string, std::string> &parsedDirectives)
{
    std::map<std::string, std::string>::const_iterator it;
    for (it = parsedDirectives.begin(); it != parsedDirectives.end(); ++it)
    {
        const std::string &directiveName = it->first;
        const std::string &directiveValue = it->second;

        if (directiveName == "listen")
        {
            this->_listen = std::atoi(directiveValue.c_str());
        }
        else if (directiveName == "autoindex")
        {
            this->_autoindex = std::atoi(directiveValue.c_str());
        }
        else if (directiveName == "client_max_body_size")
        {
            this->_client_max_body_size = std::atoi(directiveValue.c_str());
        }
        else if (directiveName == "root")
        {
            this->_root = directiveName;
        }
        else if (directiveName == "index")
        {
            std::istringstream valueStream(directiveValue);
            std::string singleValue;
            //this->index.clear();

            while (valueStream >> singleValue)
                this->_index.push_back(singleValue);
        }
        else if (directiveName == "server_name")
        {
            std::istringstream valueStream(directiveValue);
            std::string singleValue;
            //this->index.clear();

            while (valueStream >> singleValue)
                this->_server_name.push_back(singleValue);
        }
        else if (directiveName == "cgi")
        {
            std::istringstream valueStream(directiveValue);
            std::string extension, handler;
            if (valueStream >> extension >> handler)
                this->_cgi[extension] = handler;
        }
        else if (directiveName == "error_page")
        {
            std::istringstream valueStream(directiveValue);
            int errorCode;
            std::string filePath;
            if (valueStream >> errorCode >> filePath)
                this->_error_page[errorCode] = filePath;
        }
        else if (directiveName == "redirect")
        {
            std::istringstream valueStream(directiveValue);
            int redirectCode;
            std::string filePath;
            if (valueStream >> redirectCode >> filePath)
                this->_redirect[redirectCode] = filePath;
        }
    }
}




/* DEBUG */

void Server::printServerConfig(void) const
{
    std::cout << "Server Configuration:\n" << std::endl;
    std::cout << "  Listen: " << _listen << std::endl;
    std::cout << "  Autoindex: " << _autoindex << std::endl;
    std::cout << "  Client Max Body Size: " << _client_max_body_size << std::endl;
    std::cout << "  Root: " << _root << std::endl;

    std::cout << "  Index Files: ";
    for (size_t i = 0; i < _index.size(); ++i) {
        std::cout << _index[i] << (i < _index.size() - 1 ? ", " : "");
    }
    std::cout << std::endl;

    std::cout << "  Server Names: ";
    for (size_t i = 0; i < _server_name.size(); ++i) {
        std::cout << _server_name[i] << (i < _server_name.size() - 1 ? ", " : "");
    }
    std::cout << std::endl;

    std::cout << "  CGI Handlers:" << std::endl;
    for (std::map<std::string, std::string>::const_iterator it = _cgi.begin(); it != _cgi.end(); ++it) {
        std::cout << "    " << it->first << " => " << it->second << std::endl;
    }

    std::cout << "  Error Pages:" << std::endl;
    for (std::map<int, std::string>::const_iterator it = _error_page.begin(); it != _error_page.end(); ++it) {
        std::cout << "    " << it->first << " => " << it->second << std::endl;
    }

    std::cout << "  Redirects:" << std::endl;
    for (std::map<int, std::string>::const_iterator it = _redirect.begin(); it != _redirect.end(); ++it) {
        std::cout << "    " << it->first << " => " << it->second << std::endl;
    }

}

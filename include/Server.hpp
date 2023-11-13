#ifndef SERVER_HPP
#define SERVER_HPP

#include "Libs.hpp"
#include "Location.hpp"

class Server
{
    public:
        // Constructors
        Server(void);

        // Copy constructor
        Server(Server const &src);

        // Destructor
        ~Server(void);

        // Copy assignment operator
        Server &operator=(Server const &rhs);

        //methods:
        void setDirectives(const std::map<std::string, std::string> &parsedDirectives);

        //debug:
        void printServerConfig() const;


    private:
        int _listen;
        int _autoindex;
        int _client_max_body_size;
        std::string _root;
        std::vector<std::string> _index;
        std::vector<std::string> _server_name;
        std::map<std::string, std::string> _cgi;
        std::map<int, std::string> _error_page;
        std::map<int, std::string> _redirect;
        std::map<std::string, Location> _locations; 

};

#endif

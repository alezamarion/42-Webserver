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
        void setDirectives(const std::string &name, const std::string &value);

        //debug:
        void printAllServerDirectives() const;
        void printDirectives(void) const;



    private:
        int listen;
        int autoindex;
        int client_max_body_size;
        std::string root;
        std::vector<std::string> index;
        std::vector<std::string> server_name;
        std::map<std::string, std::string> cgi;
        std::map<int, std::string> error_page;
        std::pair<int, std::string> redirect;
        std::map<std::string, Location> _locations; 

};

#endif

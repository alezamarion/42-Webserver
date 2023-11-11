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
        std::map<std::string, std::string> _directives; //std::map stores its elements in a sorted order based on the key.
        std::map<std::string, Location> _locations; 

};

#endif

#ifndef Socket_HPP
#define Socket_HPP

#include "Libs.hpp"
#include "ConfigParser.hpp"

class Socket 
{
    public:
        // Constructors
        Socket(void);

        // Copy constructor
        Socket(Socket const &src);

        // Destructor
        ~Socket(void);

        // Copy assignment operator
        Socket &operator=(Socket const &rhs);
        
        //methods
        void initSockets(ConfigParser& configParser);


    private:


};


#endif

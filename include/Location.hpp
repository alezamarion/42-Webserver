#ifndef LOCATION_HPP
#define LOCATION_HPP

#include "Libs.hpp"

class Location
{
    public:
        // Constructors
        Location(void);

        // Copy constructor
        Location(Location const &src);

        // Destructor
        ~Location(void);

        // Copy assignment operator
        Location &operator=(Location const &rhs);

        //methods:


    private:
            //data:


};

#endif
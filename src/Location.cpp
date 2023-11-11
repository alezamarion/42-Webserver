#include "Location.hpp"


Location::Location(void)
{
    return;
}

Location::Location(Location const &src)
{
   *this = src; 
}

Location::~Location(void)
{
    return;
}

Location &Location::operator=(const Location &rhs)
{
    if (this != &rhs) // Check for self-assignment to avoid stack overflow
        *this = rhs; 
    return *this;
}

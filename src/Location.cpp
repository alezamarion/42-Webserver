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

Location &Location::operator=(Location const &rhs)
{
    *this = rhs; 
    return *this;
}


#ifndef ConfigSpec_HPP
#define ConfigSpec_HPP

#include "Libs.hpp"

class ConfigSpec
{
    public:
        // Constructors
        ConfigSpec(void);

        // Copy constructor
        ConfigSpec(ConfigSpec const &src);

        // Destructor
        ~ConfigSpec(void);

        // Copy assignment operator
        ConfigSpec &operator=(ConfigSpec const &rhs);

        //methods:
        void setDirectives(const std::map<std::string, std::vector<std::string> > &directives);
        void setLocationBlocks(const std::map<std::string, std::map<std::string, std::string> > &locationBlocks);

        //debug:
        void printParsedDirectives(void) const;
        void printParsedLocationBlocks(void) const;
        


    private:
    std::map<std::string, std::vector<std::string> > _directives;
    std::map<std::string, std::map<std::string, std::string> > _locationBlocks;
};

#endif


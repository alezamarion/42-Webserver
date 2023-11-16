#ifndef ConfigParser_HPP
#define ConfigParser_HPP

#include "Libs.hpp"
#include "ConfigSpec.hpp"
#include "Location.hpp"

class ConfigParser
{
    public:
        // Constructors
        ConfigParser(void);
        explicit ConfigParser(std::string const &FilePath);

        // Copy constructor
        ConfigParser(ConfigParser const &src);

        // Destructor
        ~ConfigParser(void);

        // Copy assignment operator
        ConfigParser &operator=(ConfigParser const &rhs);

        void handleConfigParserFile(char *filePath);
        bool checkBracketsMatch(void);
        bool validateServerBlock(void);
        void extractServerBlocks(void);     
        void parseServerBlocks(void);
        void parseDirectives(const std::string &ConfigSpecBlock);
        void trim(std::string &s);
        void extractLocationBlocks(std::string &ConfigSpecBlock);

        //debug:
        void printLocationBlocks(void) const;

    private:
        std::string _filePath;
        std::string _configFile;
        std::vector<std::string> _serverBlocks;
        std::map<std::string, std::string> _parsedDirectives;
        std::vector<std::string> _locationBlocks;        
        std::vector<ConfigSpec> _ConfigSpecs;
};

#endif

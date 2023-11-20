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
        void parseDirectives(const std::string &serverBlock);
        void trim(std::string &string);
        void extractLocationBlocks(std::string &serverBlock);
        void parseLocationBlocks(void);
        void extractLocationPath(const std::string  &block);
        void parseDirectivesInLocation(const std::string &block);

        //debug:
        void printParsedDirectives(void) const;
        void printLocationBlocks(void) const;
        void printLocationDirectives(void) const;
        void printParsedLocationBlocks(void) const;

    private:
        std::string _filePath;
        std::string _configFile;
        std::vector<std::string> _serverBlocks;
        std::map<std::string, std::string> _parsedDirectives;
        std::vector<std::string> _locationBlocks;  
        std::string _locationPath;
        std::map<std::string, std::string> _locationDirectives;    
        // outher key: path of location block (/website2) | innner map: map containing config directives for that location block
        std::map<std::string, std::map<std::string, std::string> > _parsedLocationBlocks; 
        std::vector<ConfigSpec> _configSpecs;
};

#endif

// {
//     "/website": {
//         {"autoindex", "off"}
//     },
//     "/website2": {
//         {"autoindex", "off"},
//         {"limit_except", "GET POST"}
//     }
// }
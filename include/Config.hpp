#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "Libs.hpp"
#include "Server.hpp"
#include "Location.hpp"

class Config
{
    public:
        // Constructors
        Config(void);
        explicit Config(std::string const &FilePath);

        // Copy constructor
        Config(Config const &src);

        // Destructor
        ~Config(void);

        // Copy assignment operator
        Config &operator=(Config const &rhs);

        void handleConfigFile(char *filePath);
        bool checkBracketsMatch(void);
        bool validateServerBlock(void);
        void extractServerBlocks(void);     
        void parseServerBlocks(void);
        void parseDirectives(const std::string &serverBlock);
        void trim(std::string &s);
        void loadDirectives(Server &server);
        std::vector<std::string> extractLocationBlocks(std::string &serverBlock);
        Location parseLocationBlock(std::string locationBlocks);

        //debug:
        void printAllServerDirectives() const;

    private:
        std::string _filePath;
        std::string _configFile;
        std::vector<std::string> _serverBlocks;
        std::map<std::string, std::string> _parsedDirectives;
        std::vector<std::string> _locationBlocks;
        std::vector<Server> _servers;
};

#endif

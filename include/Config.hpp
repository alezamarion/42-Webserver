#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "Libs.hpp"
#include "Server.hpp"

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
        void parseDirectives(const std::string &serverBlock, Server &server);
        void trim(std::string &s);
        std::vector<std::string> extractLocationBlocks(std::string &serverBlock);

        //debug:
        void printAllServerDirectives() const;

    private:
        std::string _filePath;
        std::string _configFile;
        std::vector<std::string> _serverBlocks;
        std::vector<Server> _servers;
};

#endif

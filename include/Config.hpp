#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "Libs.hpp"

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

        std::map<std::string, std::vector<std::string> > handleConfigFile(char *filePath);
        bool checkBracketsMatch(void);
        bool validateServerBlockStart(void);
        void parseConfigFile(void);     
        void getServerData(std::string serverBlock);

    private:
        std::string _filePath;
        std::string _configFile;
        std::map<std::string, std::string> _settings;

};

#endif

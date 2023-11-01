#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "Libs.hpp"

class Config
{
    public:
        // Constructors
        Config(void);
        explicit Config(std::string const &configFilePath);

        // Copy constructor
        Config(Config const &src);

        // Destructor
        ~Config(void);

        // Copy assignment operator
        Config &operator=(Config const &rhs);

        std::map<std::string, std::vector<std::string>> handleConfigFile(char *filePath);
        bool Config::checkBracketsMatch(const std::string& file_content);
        bool Config::validateServerBlockStart(const std::string& inputFile);
        

    private:
        std::string _filePath;
        std::string _configFile;
        std::map<std::string, std::string> _settings;

};

#endif

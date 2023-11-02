#include "Config.hpp"

Config::Config(void)
{
    return;
}

Config::Config(std::string const &filePath)
{
    this->_filePath = filePath;
}

Config::Config(Config const &src)
{
   *this = src; 
}

Config::~Config(void)
{
    return;
}

Config &Config::operator=(Config const &rhs)
{
    *this = rhs; 
    return *this;
}

std::map<std::string, std::vector<std::string> > Config::handleConfigFile(char *filePath)
{
    std::ifstream configFile;
    std::stringstream content;
    std::map<std::string, std::vector<std::string> > serverData;

    //open file
    configFile.open(filePath);
    if (!configFile.is_open())
    {
        throw std::runtime_error("Failed to open file: " + std::string(filePath)); //no catch block yet
    }
    std::cout << "File opened successfully!" << std::endl;

    //read the file content
    content << configFile.rdbuf();
    this->_configFile = content.str();
    configFile.close();

    //check syntax
    if (!validateServerBlockStart() || !checkBracketsMatch())
        throw std::runtime_error("syntax error detected on input file");

    std::cout << "Everything seens OK" << std::endl;

    //parse server blocks
    parseConfigFile();






    //return result in a data structure
    std::map<std::string, std::vector<std::string> > result; //empty map, just to return;
        
    return result;
}

/*  The purpose of the function checkServerBlock is to verify if the first block of the configuration file 
    starts with the word "server" followed by an opening curly brace "{"    */

bool Config::validateServerBlockStart()
{
    std::string configFileCopy = this->_configFile;
    std::string::const_iterator it = configFileCopy.begin();

    // Skip whitespaces
    while (it != configFileCopy.end() && std::isspace(*it))
        ++it;

    // Capture the potential server block name
    std::string firstBlock;
    while (it != configFileCopy.end() && *it != '{' && !std::isspace(*it))
    {
        firstBlock += *it;
        ++it;
    }

    // Skip whitespaces between the server block name and the opening brace
    while (it != configFileCopy.end() && std::isspace(*it))
        ++it;

    // Check if the next character is an opening brace
    if (it == configFileCopy.end() || *it != '{')
        return false;

    // Trim whitespaces from the server block name
    std::size_t endpos = firstBlock.find_last_not_of(" \f\n\r\t\v");
    if (std::string::npos != endpos)
        firstBlock = firstBlock.substr(0, endpos + 1);

    // Check if the block is a server block
    return firstBlock == "server";
}


/*  This function ensures that every opening brace "{" has a corresponding closing brace "}" and vice versa. 
    If it finds any mismatch, it throws an exception.   */

bool Config::checkBracketsMatch()
{
    std::stack<char> brackets;
    std::string configFileCopy = this->_configFile;

    for (std::string::iterator it = configFileCopy.begin(); it != configFileCopy.end(); ++it)
    {
        char ch = *it;
        if (ch == '{')
            brackets.push(ch);
        else if (ch == '}')
        {
            if (brackets.empty())
                throw std::runtime_error("Unexpected '}' found");
            brackets.pop();
        }
    }
    if (!brackets.empty())
        throw std::runtime_error("Unclosed '{' found");
    return true;
}

void Config::parseConfigFile(void)
{
    int braces = 0;
    size_t blockStart = 0;
    bool insideServerBlock = false;
    std::string configFileCopy = this->_configFile;

    for (size_t i = 0; i < configFileCopy.size(); i++)
    {
        char c = configFileCopy[i];
        if (c == '{')
        {
            if (!insideServerBlock)
            {
                insideServerBlock = true;
                blockStart = i;
            }
            braces++;
        }
        else if (c == '}') //here a block has founded
        {
            braces--;
            if (braces == 0)
            {
                // Extract the server block substring
                std::string serverBlock = configFileCopy.substr(blockStart, i - blockStart + 1);

                // Print the server block to verify it
                std::cout << "\nServer Block Found:\n" << serverBlock << "\n\n";
                
                //getServerData(configFileCopy.substr(blockStart, i - blockStart + 1)); //extract server block
                insideServerBlock = false;
                blockStart = i;
            }
        }
    }    
}

/*
void Config::getServerData(std::string serverBlock)
{
    Server server;
    splitOffLocationBlocks(serverBlock, server);

    serverBlock = ftstring::trim(serverBlock, " {}\n\t\v\f\r");
    std::istringstream iss(serverBlock);
    std::string        line, key;

    while (std::getline(iss, line)) {
        std::map<std::string, _parseConfigFn>::iterator it;
        line = ftstring::reduce(line, " \f\n\r\t\v");
        key  = findDirective(line);
        it   = this->_parseConfigFns.find(key);
        if (it != this->_parseConfigFns.end()) {
            server.insertServerData(it->second(line));
        }
    }
    this->_serverData.push_back(server);
}
*/
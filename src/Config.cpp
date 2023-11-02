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
    starts with the word "server" followed by an opening curly brace "{"    
    
    BUT:
    if there is another "server" block with the wrong name (example: serv), this will not be handled    
*/

bool Config::validateServerBlockStart()
{
    std::string configFileCopy = this->_configFile;

    // Find the start of first word
    std::size_t nonSpace = configFileCopy.find_first_not_of(" \f\n\r\t\v");
    if (nonSpace == std::string::npos)
        return false;  // String contains only whitespace.

    // Look for the end of the first word
    std::size_t serverWordEnd = configFileCopy.find_first_of(" \f\n\r\t\v{", nonSpace);
    if (serverWordEnd == std::string::npos)
        return false;  // There is no valid server block identifier or opening brace.

    // Extract the first word and check if it is "server"
    std::string firstWord = configFileCopy.substr(nonSpace, serverWordEnd - nonSpace);
    if (firstWord != "server")
        return false;  // The first word is not "server".

    // Find the opening brace after the "server" keyword.
    std::size_t openingBrace = configFileCopy.find_first_not_of(" \f\n\r\t\v", serverWordEnd);
    if (openingBrace == std::string::npos || configFileCopy[openingBrace] != '{')
        return false;  // There is no opening brace after the "server" keyword.

    return true;  // The first block is a server block with the correct format.
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
                
                getServerData(configFileCopy.substr(blockStart, i - blockStart + 1)); //extract server block
                insideServerBlock = false;
                blockStart = i;
            }
        }
    }    
}

void Config::getServerData(const std::string& serverBlock)
{
    std::istringstream blockStream(serverBlock);
    parseServerDirectives(blockStream); // Parse directives outside of any location block
    
    std::string locationBlock;
    while (getLocationBlocks(blockStream, locationBlock)) // Get each location block
    {   
        std::istringstream locationStream(locationBlock);
        parseLocationBlocks(locationStream); // Parse the individual location block
    }
}






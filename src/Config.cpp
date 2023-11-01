#include "Config.hpp"

Config::Config(void)
{
    return;
}

Config::Config(std::string const &configFilePath)
{
    //salvar filePath;
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
}

std::map<std::string, std::vector<std::string>> Config::handleConfigFile(char *filePath)
{
    std::ifstream configFile;
    std::stringstream content;
    std::map<std::string, std::vector<std::string>> serverData;

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
    if (!validateServerBlockStart || !checkBracketsMatch)
        throw std::runtime_error("syntax error detected in input file");

    //parse server blocks



    //return result in a data structure

}

/*  The purpose of the function checkServerBlock is to verify if the first block of the configuration file 
    starts with the word "server" followed by an opening curly brace "{"    */

bool Config::validateServerBlockStart(const std::string& inputFile)
{
    std::string::const_iterator it = inputFile.begin();

    // Skip whitespaces
    while (it != inputFile.end() && std::isspace(*it))
        ++it;

    // Capture the potential server block name
    std::string firstBlock;
    while (it != inputFile.end() && *it != '{' && !std::isspace(*it))
    {
        firstBlock += *it;
        ++it;
    }

    // Skip whitespaces between the server block name and the opening brace
    while (it != inputFile.end() && std::isspace(*it))
        ++it;

    // Check if the next character is an opening brace
    if (it == inputFile.end() || *it != '{')
        return false;

    // Trim whitespaces from the server block name
    std::size_t endpos = firstBlock.find_last_not_of(" \f\n\r\t\v");
    if (std::string::npos != endpos)
        firstBlock = firstBlock.substr(0, endpos + 1);

    // Check if the block is a server block
    return firstBlock == "server";
}


/*  This function ensures that every opening brace "{"" has a corresponding closing brace "}" and vice versa. 
    If it finds any mismatch, it throws an exception.   */

bool Config::checkBracketsMatch(const std::string& file_content)
{
    std::stack<char> brackets;
    for (char ch : file_content)
    {
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






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

Config &Config::operator=(const Config &rhs)
{
    if (this != &rhs) // Check for self-assignment to avoid stack overflow
        *this = rhs; 
    return *this;
}


/* Extract information from the configuration file and then create a server object for each server that exists in the configuration file. The following functions are part of this */

void Config::handleConfigFile(char *filePath)
{
    std::ifstream configFile;
    std::stringstream content;

    // open file
    configFile.open(filePath);
    if (!configFile.is_open())
    {
        throw std::runtime_error("Failed to open file: " + std::string(filePath)); //no catch block yet
    }
    std::cout << "File opened successfully!" << std::endl;

    // read the file content
    content << configFile.rdbuf();
    this->_configFile = content.str();
    configFile.close();

    // check syntax
    if (!validateServerBlock() || !checkBracketsMatch())
        throw std::runtime_error("syntax error detected on input file");

    std::cout << "Everything seens OK" << std::endl;

    extractServerBlocks();
    parseServerBlocks();
    // createServerObjects();
}


/* Verify if the first block of the configuration file starts with the word "server" followed by an opening curly brace "{"    
    
    BUT:
    if there is another "server" block with the wrong name (example: serv), this will not be handled    
*/

bool Config::validateServerBlock()
{
    std::string configFileCopy = this->_configFile;

    // find the start of first word
    std::size_t nonSpace = configFileCopy.find_first_not_of(" \f\n\r\t\v");
    if (nonSpace == std::string::npos)
        return false;

    // look for the end of the first word
    std::size_t serverWordEnd = configFileCopy.find_first_of(" \f\n\r\t\v{", nonSpace);
    if (serverWordEnd == std::string::npos)
        return false;

    // extract the first word and check if it is "server"
    std::string firstWord = configFileCopy.substr(nonSpace, serverWordEnd - nonSpace);
    if (firstWord != "server")
        return false;

    // find the opening brace after the "server" keyword.
    std::size_t openingBrace = configFileCopy.find_first_not_of(" \f\n\r\t\v", serverWordEnd);
    if (openingBrace == std::string::npos || configFileCopy[openingBrace] != '{')
        return false;

    return true;
}


/* Ensures that every opening brace "{" has a corresponding closing brace "}" and vice versa. 
    Also, if it finds any mismatch, it throws an exception.   */

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


/* Extract a server block from config file and stores it in a vector of server blocks */

void Config::extractServerBlocks(void)
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
        // here a block was found
        else if (c == '}') 
        {
            braces--;
            if (braces == 0)
            {
                // extract the server block substring
                this->_serverBlocks.push_back(configFileCopy.substr(blockStart, i - blockStart + 1));
                insideServerBlock = false;
                blockStart = i;
            }
        }
    } 
    // DEGUB: print the server block to verify it
    std::cout << "\nServer Block Found\n" << std::endl;
    for (size_t i = 0; i < _serverBlocks.size(); ++i)
    {
        std::cout << _serverBlocks[i] << "\n" << std::endl;
    }
}


/* Parse each server block stored in previous vector. First parse directives and then find and parse Location blocks */

void Config::parseServerBlocks()
{
    for (size_t i = 0; i < this->_serverBlocks.size(); ++i)
    {
        // create a new Server object for each server block
        Server server; 
        std::string serverBlock = this->_serverBlocks[i];

        parseDirectives(serverBlock, server);

        //DEBUG: print directives
        _servers.push_back(server);
        std::cout << "Directives for server " << (i + 1) << ":" << std::endl;
        server.printDirectives();
        std::cout << std::endl;
                

        // now find and parse all location blocks within this server block.
        // std::vector<std::string> locationBlocks = extractLocationBlocks(serverBlock, server);
        
        // for (size_t j = 0; j < locationBlocks.size(); ++j)
        // {
        //     Location location = parseLocationBlock(locationBlocks[j]);

        //     // extract the path from the location block for the key to add the Location to the Server
        //     std::istringstream iss(locationBlocks[j]);
        //     std::string locationPath;
        //     iss >> locationPath; // Extract the path

        //     // add the Location object to the Server 
        //     server.addLocation(locationPath, location); 
        // }

        // add the fully configured Server object to the vector 
        //this->_servers.push_back(server); // 
    }
}


/* This function is part of previous function parseServerBlocks, it parses all directives and store it in a Server object */

void Config::parseDirectives(const std::string &serverBlock, Server &server)
{
    std::istringstream stream(serverBlock);
    std::string line;

    // skip the opening brace
    std::getline(stream, line);

    while (std::getline(stream, line))
    {
        // remove whitespace from beginning and end of string
        trim(line); 

        // skip empty lines
        if (line.empty())
            continue;      

        // check if the line starts with "location", indicating the start of a location block
        if (line.find("location") == 0)
        {
            break; // Exit the loop, as the remaining lines are location directives
        }

        // parse the directive
        std::istringstream lineStream(line);
        std::string directive, value;
        lineStream >> directive;

        std::getline(lineStream, value);
        trim(value);

        // store the directive in the server object
        server.setDirectives(directive, value);
    }
}    


/* This function is part of previous function parseDirectives it removes leading and trailing whitespace characters from a string */

void Config::trim(std::string &s) 
{
    // find the first character that is not a whitespace
    std::string::iterator firstNonWhitespace = std::find_if(s.begin(), s.end(), not1(std::ptr_fun<int, int>(std::isspace)));

    // frase leading whitespace
    s.erase(s.begin(), firstNonWhitespace);

    // find the last character that is not a whitespace (using reverse iterator)
    std::string::reverse_iterator lastNonWhitespace = std::find_if(s.rbegin(), s.rend(), not1(std::ptr_fun<int, int>(std::isspace)));

    // erase trailing whitespace
    // convert reverse iterator to normal iterator before erasing
    s.erase(lastNonWhitespace.base(), s.end());
}


/* DEBUG */

/* Here we print all serve directives just to debug */
void Config::printAllServerDirectives() const
{
    for (size_t i = 0; i < _servers.size(); ++i)
    {
        std::cout << "Server " << (i + 1) << ":" << std::endl;
        _servers[i].printDirectives();
        std::cout << std::endl;
    }
}


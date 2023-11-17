#include "ConfigParser.hpp"

ConfigParser::ConfigParser(void)
{
    return;
}

ConfigParser::ConfigParser(std::string const &filePath)
{
    this->_filePath = filePath;
}

ConfigParser::ConfigParser(ConfigParser const &src)
{
   *this = src; 
}

ConfigParser::~ConfigParser(void)
{
    return;
}

ConfigParser &ConfigParser::operator=(const ConfigParser &rhs)
{
    if (this != &rhs) // Check for self-assignment to avoid stack overflow
        *this = rhs; 
    return *this;
}


/* Extract information from the ConfigParseruration file and then create a ConfigSpec object for each ConfigSpec that exists in the ConfigParseruration file. The following functions are part of this */

void ConfigParser::handleConfigParserFile(char *filePath)
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
}


/* Verify if the first block of the ConfigParseruration file starts with the word "ConfigSpec" followed by an opening curly brace "{"    
    
    BUT:
    if there is another "ConfigSpec" block with the wrong name (example: serv), this will not be handled    
*/

bool ConfigParser::validateServerBlock()
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

    // extract the first word and check if it is "ConfigSpec"
    std::string firstWord = configFileCopy.substr(nonSpace, serverWordEnd - nonSpace);
    if (firstWord != "Server")
        return false;

    // find the opening brace after the "ConfigSpec" keyword.
    std::size_t openingBrace = configFileCopy.find_first_not_of(" \f\n\r\t\v", serverWordEnd);
    if (openingBrace == std::string::npos || configFileCopy[openingBrace] != '{')
        return false;

    return true;
}


/* Ensures that every opening brace "{" has a corresponding closing brace "}" and vice versa. 
    Also, if it finds any mismatch, it throws an exception.   */

bool ConfigParser::checkBracketsMatch()
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


/* Extract a ConfigSpec block from ConfigParser file and stores it in a vector of ConfigSpec blocks */

void ConfigParser::extractServerBlocks(void)
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
                // extract the ConfigSpec block substring
                this->_serverBlocks.push_back(configFileCopy.substr(blockStart, i - blockStart + 1));
                insideServerBlock = false;
                blockStart = i;
            }
        }
    } 
    // DEGUB: print the ConfigSpec block to verify it
    std::cout << "\nServer Block Found\n" << std::endl;
    for (size_t i = 0; i < _serverBlocks.size(); ++i)
    {
        std::cout << _serverBlocks[i] << "\n" << std::endl;
    }
}


/* Parse each ConfigSpec block stored in previous vector. First parse directives and then find and parse Location blocks */

void ConfigParser::parseServerBlocks()
{
    _configSpecs.clear(); 

    for (size_t i = 0; i < this->_serverBlocks.size(); ++i)
    {
        ConfigSpec configSpec;
        std::string serverBlock = this->_serverBlocks[i];

        parseDirectives(serverBlock);
        //DEBUG
        printParsedDirectives();

        extractLocationBlocks(serverBlock);
        //DEBUG:
        printLocationBlocks();

        parseLocationBlocks();
        //DEBUG:
        printParsedLocationBlocks();
        
        // transfer parsed data to ConfigSpec object;
        //configSpec.setdirectives(_parsedDirectives)
        // configSpec.setLocationBlocks(_parsedLocationBlocks);

        // add the fully populated ConfigSpec object to the vector      
        // _configSpecs.push_back(configSpec);
    }
}



/* This function is part of previous function parseConfigSpecBlocks, it parses all directives and store it in a ConfigSpec object */

void ConfigParser::parseDirectives(const std::string &serverBlock)
{
    std::istringstream stream(serverBlock);
    std::string line;

    // skip the opening brace
    std::getline(stream, line);

    _parsedDirectives.clear();

    while (std::getline(stream, line))
    {
        // remove whitespace from beginning and end of string
        trim(line); 

        // skip empty lines
        if (line.empty())
            continue;      

        // check if the line starts with "location", indicating the start of a location block
        if (line.find("location") == 0)
            break; // Exit the loop, as the remaining lines are location directives

        // parse the directive
        std::istringstream lineStream(line);
        std::string directive, value;
        lineStream >> directive;

        std::getline(lineStream, value);
        trim(value);

        // store the directive in the ConfigSpec object
        _parsedDirectives[directive] = value;
    }
}    


/* This function is part of previous function parseDirectives it removes leading and trailing whitespace characters from a string */

void ConfigParser::trim(std::string &string) 
{
    // Check if the string is empty
    if (string.empty())
        return;

    // find the first character that is not a whitespace
    std::string::iterator firstNonWhitespace = std::find_if(string.begin(), string.end(), not1(std::ptr_fun<int, int>(std::isspace)));

    // frase leading whitespace
    string.erase(string.begin(), firstNonWhitespace);

    // find the last character that is not a whitespace (using reverse iterator)
    std::string::reverse_iterator lastNonWhitespace = std::find_if(string.rbegin(), string.rend(), not1(std::ptr_fun<int, int>(std::isspace)));

    // erase trailing whitespace
    // convert reverse iterator to normal iterator before erasing
    string.erase(lastNonWhitespace.base(), string.end());
}



/* Extract all location blocks inside ConfigParseruration file and store it in a string vector */

void ConfigParser::extractLocationBlocks(std::string &serverBlock)
{
    std::size_t startPos = 0;
    startPos = serverBlock.find("location", startPos);

    _locationBlocks.clear();

    while (startPos != std::string::npos)
    {
        // find the opening curly brace of the location block
        std::size_t braceOpen = serverBlock.find('{', startPos);
        
        // error handling if '{' is not found
        if (braceOpen == std::string::npos)
            break; 

        // find the closing curly brace, considering nested braces
        int braceCount = 1;
        std::size_t braceClose = braceOpen;
        while (braceCount > 0 && ++braceClose < serverBlock.length())
        {
            if (serverBlock[braceClose] == '{')
                braceCount++;
            else if (serverBlock[braceClose] == '}')
                braceCount--;
        }

        // error handling for unmatched brace
        if (braceCount != 0)
            break;

        // extract the location block
        std::string locationBlock = serverBlock.substr(startPos, braceClose - startPos + 1);
        _locationBlocks.push_back(locationBlock);

        // move to the next part of the string
        startPos = braceClose + 1;
    }
}



/* Parse location blocks and store id in */

void ConfigParser::parseLocationBlocks(void)
{
    _parsedLocationBlocks.clear();

    for (size_t i = 0; i < _locationBlocks.size(); ++i)
    {
        std::string block = _locationBlocks[i];

        // extract location path from the block
        extractLocationPath(block);

        // parse directives within this block
        parseDirectivesInLocation(block);

        // store the directives map in the _parsedLocationBlocks
        _parsedLocationBlocks[_locationPath] = _locationDirectives;
    }
}

void ConfigParser::extractLocationPath(const std::string& block)
{
    _locationPath.clear();

    std::istringstream stream(block);
    std::string line;

    while (std::getline(stream, line))
    {
        trim(line);

        if (line.empty())
            continue;

        // the first line should start with "location", followed by the path.
        std::size_t locationStart = line.find("location");
        if (locationStart != std::string::npos)
        {
            std::size_t pathStart = line.find_first_not_of(" ", locationStart + 8); // 8 is the length of "location"
            std::size_t pathEnd = line.find_first_of(" {", pathStart);

            if (pathStart != std::string::npos && pathEnd != std::string::npos)
            {
                _locationPath = line.substr(pathStart, pathEnd - pathStart);
                return;
            }
        }
        throw std::runtime_error("Location path not found in the location block");
    }
     throw std::runtime_error("Empty location block encountered");
}


void ConfigParser::parseDirectivesInLocation(const std::string &block)
{
    _locationDirectives.clear();

    std::istringstream stream(block);
    std::string line;

    // skip the first line as it contains the location path
    std::getline(stream, line);

    while (std::getline(stream, line))
    {
        // trim whitespace
        trim(line); 

        // skip empty lines and the closing brace
        if (line.empty() || line == "}")
            continue;

        std::istringstream lineStream(line);
        std::string directive, value;

        // get the directive name
        lineStream >> directive;

        // read the rest of the line as the directive value
        std::getline(lineStream, value);
        trim(value);

        // store the directive and its value in the map
        _locationDirectives[directive] = value;
    }
}


/* DEBUG */

void ConfigParser::printParsedDirectives(void) const
{
    std::cout << "Parsed Directives:" << std::endl;
    for (std::map<std::string, std::string>::const_iterator it = _parsedDirectives.begin(); it != _parsedDirectives.end(); ++it)
        std::cout << "  " << it->first << ": " << it->second << std::endl;
}




void ConfigParser::printLocationBlocks(void) const
{
    std::cout << "\nExtracted Location Blocks:\n" << std::endl;
    for (size_t i = 0; i < _locationBlocks.size(); ++i)
    {
        std::cout << "Location Block " << (i + 1) << ":" << std::endl;
        std::cout << _locationBlocks[i] << std::endl;
    }
}


void ConfigParser::printParsedLocationBlocks(void) const
{
    std::cout << "Parsed Location Blocks:" << std::endl;
    for (std::map<std::string, std::map<std::string, std::string> >::const_iterator it = _parsedLocationBlocks.begin(); it != _parsedLocationBlocks.end(); ++it)
    {
        std::cout << "Location: " << it->first << std::endl;

        const std::map<std::string, std::string>& directives = it->second;
        for (std::map<std::string, std::string>::const_iterator innerIt = directives.begin(); innerIt != directives.end(); ++innerIt)
            std::cout << "  " << innerIt->first << ": " << innerIt->second << std::endl;
        std::cout << std::endl;
    }    
}

/*

location blocks:

    location /website {
        autoindex off
    }

    location /website2 {
        autoindex off
        limit_except GET POST
        error_page 500 custom_500.html
    }

onde armazenar:

1 - std::map<std::string, std::map<std::string, std::vector<std::string>>>

   [ /website2 ] [ limit_except ] [ get ] [ post ] 

    - mais dificil par armazenar, mais facil para obter as informacoes

2 - std::map<std::string, std::map<std::string, std::string>>

    [ /website ] [ limit_except ] [ get post ]

    - mais facil de armazenar, mais dificil para obter as informacoes
    - teria que cortar a string " get post " para obter os dois valores
    - por outro lado so duas diretrizes tem multiplos valores no location: 
        - limit_except
        - error_page
















*/
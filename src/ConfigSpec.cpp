#include "ConfigSpec.hpp"


ConfigSpec::ConfigSpec(void)
{
    return;
}

ConfigSpec::ConfigSpec(ConfigSpec const &src)
{
   *this = src; 
}

ConfigSpec::~ConfigSpec(void)
{
    return;
}

ConfigSpec& ConfigSpec::operator=(const ConfigSpec& rhs)
{
    if (this != &rhs) // Check for self-assignment avoiding stack overflow
    { 
        *this = rhs;       
    }
    return *this;
}

void ConfigSpec::setDirectives(const std::map<std::string, std::string> &parsedDirectives)
{
    std::map<std::string, std::string>::const_iterator it;
    for (it = parsedDirectives.begin(); it != parsedDirectives.end(); ++it)
    {
        const std::string &directiveName = it->first;
        const std::string &directiveValue = it->second;

        if (directiveName == "listen")
        {
            this->_listen = std::atoi(directiveValue.c_str());
        }
        else if (directiveName == "autoindex")
        {
            this->_autoindex = std::atoi(directiveValue.c_str());
        }
        else if (directiveName == "client_max_body_size")
        {
            this->_client_max_body_size = std::atoi(directiveValue.c_str());
        }
        else if (directiveName == "root")
        {
            this->_root = directiveName;
        }
        else if (directiveName == "index")
        {
            std::istringstream valueStream(directiveValue);
            std::string singleValue;
            //this->index.clear();

            while (valueStream >> singleValue)
                this->_index.push_back(singleValue);
        }
        else if (directiveName == "ConfigSpec_name")
        {
            std::istringstream valueStream(directiveValue);
            std::string singleValue;
            //this->index.clear();

            while (valueStream >> singleValue)
                this->_ConfigSpec_name.push_back(singleValue);
        }
        else if (directiveName == "cgi")
        {
            std::istringstream valueStream(directiveValue);
            std::string extension, handler;
            if (valueStream >> extension >> handler)
                this->_cgi[extension] = handler;
        }
        else if (directiveName == "error_page")
        {
            std::istringstream valueStream(directiveValue);
            int errorCode;
            std::string filePath;
            if (valueStream >> errorCode >> filePath)
                this->_error_page[errorCode] = filePath;
        }
        else if (directiveName == "redirect")
        {
            std::istringstream valueStream(directiveValue);
            int redirectCode;
            std::string filePath;
            if (valueStream >> redirectCode >> filePath)
                this->_redirect[redirectCode] = filePath;
        }
    }
}




/* DEBUG */


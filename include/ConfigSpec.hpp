#ifndef ConfigSpec_HPP
#define ConfigSpec_HPP

#include "Libs.hpp"

class ConfigSpec
{
    public:
        // Constructors
        ConfigSpec(void);

        // Copy constructor
        ConfigSpec(ConfigSpec const &src);

        // Destructor
        ~ConfigSpec(void);

        // Copy assignment operator
        ConfigSpec &operator=(ConfigSpec const &rhs);

        //methods:
        void setDirectives(const std::map<std::string, std::string> &parsedDirectives);

        //debug:



    private:
        std::map<std::string, std::string> _parsedDirectives;

        int _listen;
        int _autoindex;
        int _client_max_body_size;
        std::string _root;
        std::vector<std::string> _index;
        std::vector<std::string> _ConfigSpec_name;
        std::map<std::string, std::string> _cgi;
        std::map<int, std::string> _error_page;
        std::map<int, std::string> _redirect;

};

#endif


//- classe ConfigParser -> parseia o arquivo e gera 1 ConfigParser spec para cada ConfigSpec
//- classe ConfigSpec; -> os getters fazer o parse getters diretivas e os getters do location


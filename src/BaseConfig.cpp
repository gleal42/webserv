#include "webserver.hpp"

// Constructor
BaseConfig::BaseConfig() : _autoindex(AUTOINDEX_UNSET), _client_max_body_size(-1)
{};

// Destructor
BaseConfig::~BaseConfig()
{};

// ServerConfig utils
namespace {
    bool valid_error_code(int code)
    {
    return (code == 100 || code == 101 || code == 200 || code == 201
         || code == 202 || code == 203 || code == 204 || code == 205
         || code == 206 || code == 207 || code == 300 || code == 301
         || code == 302 || code == 303 || code == 304 || code == 305
         || code == 307 || code == 400 || code == 401 || code == 402
         || code == 403 || code == 404 || code == 405 || code == 406
         || code == 407 || code == 408 || code == 409 || code == 410
         || code == 411 || code == 412 || code == 413 || code == 414
         || code == 415 || code == 416 || code == 417 || code == 422
         || code == 423 || code == 424 || code == 426 || code == 428
         || code == 429 || code == 431 || code == 451 || code == 500
         || code == 501 || code == 502 || code == 503 || code == 504
         || code == 505 || code == 507 || code == 511);
    }

    bool is_number(const std::string& str)
    {
        return (!str.empty() && (str.find_first_not_of("[0123456789]") == std::string::npos));
    }
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~BaseConfig methods~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// BaseConfig Setters
// void BaseConfig::set_directive(int directive, std::string& content)

void    BaseConfig::set_root(bool has_separators, const std::string &content)
{
    if (has_separators)
	    throw (MultipleArgumentsError(content));
    else if (!is_directory("public" + content))
	    throw (BadDirectoryError(content));
    this->_root = content;
}

void    BaseConfig::set_autoindex(const std::string &content)
{
    if (content == "on")
        this->_autoindex = AUTOINDEX_ON;
    else if (content == "off")
        this->_autoindex = AUTOINDEX_OFF;
    else
	    throw (ConfigurationDirectiveError(content));
}

void    BaseConfig::set_error_pages(const std::string &content)
{
    size_t              found = content.find_last_of(SEPARATORS);

    if (found == std::string::npos)
	    throw (MultipleArgumentsError(content));

    std::string         error_path = content.substr(found + 1);
    std::string         codes = content.substr(0, found);
    std::stringstream   stoi;
    int                 converted_number;
    char*               token = std::strtok(const_cast<char *>(codes.c_str()), SEPARATORS);
    size_t              i = 0;

    while (token != 0 && i++ < found)
    {
        stoi.clear();
        stoi << token;
        stoi >> converted_number;
        if (!valid_error_code(converted_number))
	        throw (ConfigurationDirectiveError(content));
        this->_error_pages[error_path].push_back(converted_number);
        token = std::strtok(NULL, SEPARATORS);
    }
}

void    BaseConfig::set_max_body_size(bool has_separators, const std::string &content)
{
    if (has_separators)
	    throw (MultipleArgumentsError(content));
    int                 max_size;
    std::stringstream   intValue(content);
    intValue >> max_size;
    if (max_size < 0)
        throw (ConfigurationDirectiveError(content));
    this->_client_max_body_size = max_size;
}

void    BaseConfig::set_indexes(const std::string &content)
{
    std::string tmp(content);
    char*       token = strtok(const_cast<char *>(tmp.c_str()), SEPARATORS);

    while (token != NULL)
    {
        std::string tokstr = std::string(token);
        this->_indexes.push_back(tokstr);
        token = strtok(NULL, SEPARATORS);
    }
}

void    BaseConfig::set_redirect(bool has_separators, const std::string &content)
{
    Redirect    new_redir;
    std::string str;

    if (has_separators)
    {
        std::stringstream   stoi;
        int                 converted_number;
        str = content.substr(0, 3);
        if (!is_number(str))
            throw (ConfigurationDirectiveError(str));
        stoi << str;
        stoi >> converted_number;
        new_redir.code = converted_number;
        str.clear();
        str = content.substr(3);
        if (!is_directory(str) || !is_file(str))
            throw (ConfigurationDirectiveError(str));
        new_redir.new_path = str;
    }
    else
    {
        if (!is_directory(content) || !is_file(content))
            throw (ConfigurationDirectiveError(str));
        new_redir.new_path = content;
        new_redir.code = 302;
    }
    this->_redirect.push_back(new_redir);
}


// BaseConfig Getters
const std::string&              BaseConfig::get_root( void ) const{return (this->_root);}
AutoBool                        BaseConfig::get_autoindex( void ) const{return (this->_autoindex);}
const ErrorPage&                BaseConfig::get_error_pages( void ) const{return (this->_error_pages);}
long long                       BaseConfig::get_max_body_size( void ) const {return (this->_client_max_body_size);}
const std::vector<std::string>& BaseConfig::get_indexes( void ) const{return (this->_indexes);}
const std::vector<Redirect>&    BaseConfig::get_redirect( void ){return (this->_redirect);}


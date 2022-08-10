#include "ServerConfig.hpp"
#include "webserver.hpp"
#include "ConfigParser.hpp"

// Constructors
BaseConfig::BaseConfig() : _autoindex(unset), _client_max_body_size(-1){}

BaseConfig::~BaseConfig(){}

ServerConfig::ServerConfig() : _ip("127.0.0.1"),_port(8080){}

// ServerConfig utils
namespace {
    bool valid_error_code(int code){
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

    bool valid_method(std::string str)
    {
        return (str == "GET" || str == "POST" || str == "DELETE"
                || str == "PUT");
    }

    bool is_number(const std::string& str){
        return (!str.empty() && (str.find_first_not_of("[0123456789]") == std::string::npos));
    }

    std::vector<std::string> split(const std::string& str, char delim){
        size_t                      i = 0;
        std::vector<std::string>    vec;
        size_t                      pos = str.find(delim);

        while (pos != std::string::npos){
            vec.push_back(str.substr(i, pos - i));
            i = ++pos;
            pos = str.find(delim, pos);
        }
        vec.push_back(str.substr(i, str.length()));
        return (vec);
    }

    bool valid_ip(std::string ip){
        if (ip == "0.0.0.0" || ip == "localhost" || ip == "*")
            return (true);

        std::vector<std::string> ip_vec          = split(ip, '.');

        if (ip_vec.size() != 4)
            return (false);

        int                                      i;
        std::stringstream                        stoi;
        std::vector<std::string>::iterator it    = ip_vec.begin();
        for (; it != ip_vec.end(); it++){
                stoi.clear();
                stoi << *it;
                stoi >> i;
            if (!is_number(*it) || i < 0 || i > 255)
                return (false);
        }
        return (true);
    }
}
// ~~~~~~~~~~~~~~~~~~~~~~~~~~BaseConfig methods~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// BaseConfig Setters
void    BaseConfig::set_root(bool has_separators, std::string &content){
    if (has_separators)
	    throw (MultipleArgumentsError(content));
    else if (!is_directory(content))
	    throw (BadDirectoryError(content));
    this->_root = content;
}

void    BaseConfig::set_autoindex(std::string &content){
    if (content == "on")
        this->_autoindex = on;
    else if (content == "off")
        this->_autoindex = off;
    else
	    throw (ConfigurationDirectiveError(content));
}

void    BaseConfig::set_error_pages(std::string &content){
    size_t              found = content.find_last_of(SEPARATORS);

    if (found == std::string::npos)
	    throw (MultipleArgumentsError(content));

    std::string         error_path = content.substr(found + 1);
    if (!(is_directory(error_path)))
	    throw (BadDirectoryError(error_path));
    std::string         codes = content.substr(0, found);
    std::stringstream   stoi;
    int                 converted_number;
    char*               token = std::strtok(const_cast<char *>(codes.c_str()), SEPARATORS);
    size_t              i = 0;

    while (token != 0 && i++ < found){
        stoi.clear();
        stoi << token;
        stoi >> converted_number;
        if (!valid_error_code(converted_number))
	        throw (ConfigurationDirectiveError(content));
        this->_error_pages[error_path].push_back(converted_number);
        token = std::strtok(NULL, SEPARATORS);
    }
}

void    BaseConfig::set_max_body_size(int has_separators,std::string &content){
    if (has_separators)
	    throw (MultipleArgumentsError(content));
    int                 max_size;
    std::stringstream   intValue(content);
    intValue >> max_size;
    if (max_size < 0)
        throw (ConfigurationDirectiveError(content));
    this->_client_max_body_size = max_size;
}

void    BaseConfig::set_indexes(const std::string &content){
    std::string tmp(content);
    char*       token = strtok(const_cast<char *>(tmp.c_str()), SEPARATORS);
    while (token != NULL){
        this->_indexes.push_back(token);
        token = strtok(NULL, SEPARATORS);
    }
}

// BaseConfig Getters
std::string                 BaseConfig::get_root( void ) const {return (this->_root);}
autobool                    BaseConfig::get_autoindex( void ) const  {return (this->_autoindex);}
errorPage                   BaseConfig::get_error_pages( void ) const  {return (this->_error_pages);}
int                         BaseConfig::get_max_body_size( void ) const  {return (this->_client_max_body_size);}
std::vector<std::string>    BaseConfig::get_indexes( void ) const  {return (this->_indexes);}


//~~~~~~~~~~~~~~~~~~~~~~~~LocationConfig methods~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

bool    LocationConfig::is_empty( void ){
    return (this->_root.empty() && this->_autoindex == unset
        && this->_error_pages.empty() && this->_client_max_body_size == -1
        && this->_indexes.empty() && this->_limit_except.empty()
        && this->_cgi.empty());
}

// LocationConfig setters

void LocationConfig::set_directive(int directive, std::string& content){
    bool    has_separators = (content.find(SEPARATORS) != std::string::npos);
    if (content.empty())
        throw (ConfigurationSyntaxError());

    switch (directive)
    {
        case DIRECTIVE_ROOT:
            this->set_root(has_separators, content);
            break;
        case DIRECTIVE_AUTOINDEX:
            this->set_autoindex(content);
            break;
        case DIRECTIVE_ERRORPAGE:
            this->set_error_pages(content);
            break;
        case DIRECTIVE_MAXBODYSIZE:
            this->set_max_body_size(has_separators, content);
            break;
        case DIRECTIVE_INDEX:
            this->set_indexes(content);
            break;
        case DIRECTIVE_LIMITEXCEPT:
            this->set_limit_except(content);
            break;
        case DIRECTIVE_CGI:
            this->set_cgi(content);
            break;
        default:
            break;
    }
};

void    LocationConfig::set_cgi(std::string &content)
{
    this->_cgi = content;
}


void    LocationConfig::set_limit_except(const std::string &content){
    char *token = std::strtok(const_cast<char *>(content.c_str()), " ");

    while (token){
        if (!(valid_method(token)))
	        throw (ConfigurationDirectiveError(content));
        std::vector<std::string>::iterator last = this->_limit_except.end();
        std::vector<std::string>::iterator tmp = this->_limit_except.begin();
        for (; tmp != last; ++tmp)
            if (*tmp == token)
	            throw (ConfigurationDirectiveError(content));
        this->_limit_except.push_back(std::string(token));
        token = std::strtok(NULL, " ");
    }
}

// LocationConfig getters
std::string                 LocationConfig::get_cgi( void ) {return (this->_cgi);}
std::vector<std::string>    LocationConfig::get_limit_except( void ) {return (this->_limit_except);}


//~~~~~~~~~~~~~~~~~~~~~~~~ServerConfig methods~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// ServerConfig utils
bool    ServerConfig::is_empty( void ){
    return (this->_root.empty() && this->_autoindex == unset
        && this->_error_pages.empty() && this->_client_max_body_size == -1
        && this->_indexes.empty() && this->_ip == "127.0.0.1"
        && this->_port == 8080 && this->_server_name.empty()
        && this->_locations.empty());
}
// ServerConfig setters
void ServerConfig::set_directive(int directive, std::string& content){
    bool    has_separators = (content.find(SEPARATORS) != std::string::npos);
    if (content.empty())
        throw (ConfigurationSyntaxError());

    switch (directive)
    {
        case DIRECTIVE_ROOT:
            this->set_root(has_separators, content);
            break;
        case DIRECTIVE_AUTOINDEX:
            this->set_autoindex(content);
            break;
        case DIRECTIVE_ERRORPAGE:
            this->set_error_pages(content);
            break;
        case DIRECTIVE_MAXBODYSIZE:
            this->set_max_body_size(has_separators, content);
            break;
        case DIRECTIVE_INDEX:
            this->set_indexes(content);
            break;
        case DIRECTIVE_LISTEN:
            this->set_listen(has_separators, content);
            break;
        case DIRECTIVE_SERVERNAME:
            this->set_server_name(content);
            break;
        default:
            break;
    }
};

void    ServerConfig::set_listen(int has_separators, const std::string &content){
    if (has_separators)
	    throw (std::runtime_error(content + ": This directive can only have one argument"));

    std::stringstream   stoi;

    if (content.find(':') != std::string::npos){
        std::string ip_str = content.substr(0, content.find(':'));
        std::string port_str = content.substr(content.find(':') + 1);
        if (!valid_ip(ip_str) || port_str.empty())
	        throw (ConfigurationDirectiveError(content));
        else if (ip_str == "*")
            this->_ip = "0.0.0.0";
        else if (ip_str == "localhost")
            this->_ip = "127.0.0.1";
        else
            this->_ip = ip_str;
        stoi << port_str;
        stoi >> this->_port;
    }
    else if (valid_ip(content)){
        if (content == "*")
            this->_ip = "0.0.0.0";
        else if (content == "localhost")
            this->_ip = "127.0.0.1";
        else
            this->_ip = content;
    }
    else
    {
        stoi << content;
        stoi >> this->_port;
        if (this->_port > PORT_MAX || this->_port <= PORT_MIN)
	        throw (ConfigurationDirectiveError(content));
    }
}

void    ServerConfig::set_server_name(const std::string &content){
    std::string tmp(content);
    char *token = strtok(const_cast<char *>(tmp.c_str()), SEPARATORS);

    while (token != NULL){
        this->_server_name.push_back(token);
        token = strtok(NULL, SEPARATORS);
    }
}

std::string                 ServerConfig::get_ip( void )
{
	return (this->_ip);
}
int                         ServerConfig::get_port( void )
{
	return (this->_port);
}
std::vector<std::string>    ServerConfig::get_server_name( void )
{
	return (this->_server_name);
}
Locations                   ServerConfig::get_locations( void )
{
	return (this->_locations);
}

void	ServerConfig::set_location( const std::string &name, const LocationConfig &location )
{
    _locations[name]=location;
}
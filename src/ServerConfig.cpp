#include "ServerConfig.hpp"
#include "ConfigParser.hpp"

// Constructors
BaseConfig::BaseConfig() : _autoindex(AUTOINDEX_UNSET), _client_max_body_size(-1){};
LocationConfig::LocationConfig(){std::cout << "DEF\n";};
ServerConfig::ServerConfig() : _ip("127.0.0.1"),_port(8080){};

// Destructors
BaseConfig::~BaseConfig(){};
LocationConfig::~LocationConfig(){};
ServerConfig::~ServerConfig(){};

// Copy constructor
ServerConfig::ServerConfig(const ServerConfig& param){
    this->_root = param._root;
    this->_autoindex = param._autoindex;
    this->_error_pages = param._error_pages;
    this->_client_max_body_size = param._client_max_body_size;
    this->_indexes = param._indexes;
    this->_locations = param._locations;
}

LocationConfig::LocationConfig(const LocationConfig& param){
    this->_root = param._root;
    this->_autoindex = param._autoindex;
    this->_error_pages = param._error_pages;
    this->_client_max_body_size = param._client_max_body_size;
    this->_indexes = param._indexes;
    this->_cgi = param._cgi;
    this->_limit_except = param._limit_except;
}

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

    bool valid_method(std::string str){
        return (str == "GET" || str == "POST" || str == "DELETE");}

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
// void BaseConfig::set_directive(int directive, std::string& content)

void    BaseConfig::set_root(bool has_separators, std::string &content){
    if (has_separators)
	    throw (MultipleArgumentsError(content));
    else if (!is_directory(content))
	    throw (BadDirectoryError(content));
    this->_root = content;
}

void    BaseConfig::set_autoindex(std::string &content){
    if (content == "on")
        this->_autoindex = AUTOINDEX_ON;
    else if (content == "off")
        this->_autoindex = AUTOINDEX_OFF;
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

void    BaseConfig::set_max_body_size(bool has_separators,std::string &content){
    if (has_separators)
	    throw (MultipleArgumentsError(content));
    int                 max_size;
    std::stringstream   intValue(content);
    intValue >> max_size;
    if (max_size < 0)
        throw (ConfigurationDirectiveError(content));
    this->_client_max_body_size = max_size;
}

void    BaseConfig::set_indexes(std::string &content){
    std::string tmp(content);
    char*       token = strtok(const_cast<char *>(tmp.c_str()), SEPARATORS);
    while (token != NULL){
        this->_indexes.push_back(token);
        token = strtok(NULL, SEPARATORS);
    }
}

// BaseConfig Getters
std::string                 BaseConfig::get_root( void ) {return (this->_root);}
autobool                    BaseConfig::get_autoindex( void ) {return (this->_autoindex);}
errorPage                   BaseConfig::get_error_pages( void ) {return (this->_error_pages);}
int                         BaseConfig::get_max_body_size( void ) const {return (this->_client_max_body_size);}
std::vector<std::string>    BaseConfig::get_indexes( void ) {return (this->_indexes);}


//~~~~~~~~~~~~~~~~~~~~~~~~LocationConfig methods~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

bool    LocationConfig::is_empty( void ){
    return (this->_root.empty() && this->_autoindex == AUTOINDEX_UNSET
        && this->_error_pages.empty() && this->_client_max_body_size == -1
        && this->_indexes.empty() && this->_limit_except.empty()
        && this->_cgi.empty());
}

int     LocationConfig::find_directive(std::string &directive){
    const std::string   valid_location_directives[CONTEXT_DIRECTIVES] =
    {"root", "autoindex", "error_page", "client_max_body_size", "index",
    "limit_except","cgi"};

    int                 i = -1;

    while (++i < CONTEXT_DIRECTIVES)
        if (directive == valid_location_directives[i])
            return (i);
    throw (ConfigurationDirectiveError(directive));
    return (0);
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
        case DIRECTIVE_CGI:
         this->set_cgi(has_separators, content);
            break;
        case DIRECTIVE_LIMITEXCEPT:
            this->set_limit_except(content);
            break;
        default:
            break;
    }
}

void    LocationConfig::set_cgi(bool has_separators, std::string &content)
{
    if (has_separators)
	    throw (MultipleArgumentsError(content));
    else if (!is_directory(content))
	    throw (BadDirectoryError(content));
    this->_cgi = content;
}

void    LocationConfig::set_limit_except(std::string &content){
    char *token = std::strtok(const_cast<char *>(content.c_str()), " ");

    while (token){
        if (!(valid_method(token)))
	        throw (ConfigurationDirectiveError(token));
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
    return (this->_root.empty() && this->_autoindex == AUTOINDEX_UNSET
        && this->_error_pages.empty() && this->_client_max_body_size == -1
        && this->_indexes.empty() && this->_ip == "127.0.0.1"
        && this->_port == 8080 && this->_server_name.empty()
        && this->_locations.empty());
}

int     ServerConfig::find_directive(std::string &directive){
    const std::string   valid_server_directives[CONTEXT_DIRECTIVES] =
    {"root","autoindex", "error_page", "client_max_body_size", "index",
    "listen","server_name"};

    int                 i = -1;

    while (++i < CONTEXT_DIRECTIVES)
        if (directive == valid_server_directives[i])
            return (i);
    throw (ConfigurationDirectiveError(directive));
    return (0);
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
void    ServerConfig::set_listen(bool has_separators, std::string &content){
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

void    ServerConfig::set_server_name(std::string &content){
    std::string tmp(content);
    char *token = strtok(const_cast<char *>(tmp.c_str()), SEPARATORS);

    while (token != NULL){
        this->_server_name.push_back(token);
        token = strtok(NULL, SEPARATORS);
    }
}

// ServerConfig Getters

std::string                 ServerConfig::get_ip( void ) {return (this->_ip);}
int                         ServerConfig::get_port( void ) {return (this->_port);}
Locations                   ServerConfig::get_locations( void ) {return (this->_locations);}
std::vector<std::string>    ServerConfig::get_server_name( void ) {return (this->_server_name);}


// Operators
ServerConfig& ServerConfig::operator= (const ServerConfig& param){
    if (this == &param)
        return (*this);

    this->_root = param._root;
    this->_autoindex = param._autoindex;
    this->_error_pages = param._error_pages;
    this->_client_max_body_size = param._client_max_body_size;
    this->_indexes = param._indexes;
    this->_ip = param._ip;
    this->_port = param._port;
    this->_server_name = param._server_name;
    this->_locations = param._locations;

    return (*this);
}

LocationConfig& LocationConfig::operator= (const LocationConfig& param){
    if (this == &param)
        return (*this);

    this->_root = param._root;
    this->_autoindex = param._autoindex;
    this->_error_pages = param._error_pages;
    this->_client_max_body_size = param._client_max_body_size;
    this->_indexes = param._indexes;
    this->_cgi = param._cgi;
    this->_limit_except = param._limit_except;

    return (*this);
}

std::ostream& operator<<(std::ostream & s, ServerConfig & param) {
    s << "|**********************************************|"<< std::endl;
    if (!(param.get_ip().empty())) s << "| Server " << param.get_ip() << ":" << param.get_port() << std::endl;
    if (!(param.get_root().empty()))  s << "| - root path " << param.get_root() << std::endl;
    if (!(param.get_error_pages().empty())){
        s << "| - errors map [" << std::endl;
        std::map<std::string, std::vector<int> >::const_iterator it_m = param.get_error_pages().begin();
        std::vector<int>::const_iterator it_v;
        for (; it_m != param.get_error_pages().end() ; it_m++) {
            s << "|     " << it_m->first << ": [";
            for (it_v = (it_m->second).begin() ; it_v != it_m->second.end() ; it_v++)
                s << *it_v << " ";
            s << "]" << std::endl;
        }
        std::cout << "|   ]" << std::endl;
    }
    if (!(param.get_ip().empty())){
        s << "| - server names [";
        std::vector<std::string> s_list(param.get_server_name());
        std::vector<std::string>::iterator s_it(s_list.begin());
        for (; s_it != s_list.end() ; ++s_it)
            s << *s_it << " ";
        s << "]" << std::endl;
    }
    if (param.get_autoindex() != AUTOINDEX_UNSET) s << "| - autoindex " << (param.get_autoindex() == AUTOINDEX_ON ? "on" : "off") << std::endl;
    if (param.get_max_body_size() != -1) s << "| - max_body_size " << param.get_max_body_size() << std::endl;
    if (!(param.get_locations().empty())){
        s << "| - [" << param.get_locations().size() << "] locations " << std::endl;
        Locations list = param.get_locations();
        Locations::iterator it(list.begin());
        for (; it != list.end() ; ++it) {
            if (!(param.get_root().empty())) s << "|     root path " << it->second.get_root() << std::endl;
            if (!(param.get_error_pages().empty())){
                s << "|     errors map [" << std::endl;
                std::map<std::string, std::vector<int> >::const_iterator it_m = it->second.get_error_pages().begin();
                std::vector<int>::const_iterator it_v;
                for (; it_m !=  it->second.get_error_pages().end() ; it_m++) {
                    s << "|       " << it_m->first << ": [";
                    for (it_v = (it_m->second).begin() ; it_v != it_m->second.end() ; it_v++)
                        s << *it_v << " ";
                    s << "]" << std::endl;
                }
                std::cout << "|     ]" << std::endl;
            }
            if (it->second.get_autoindex() != AUTOINDEX_UNSET)s << "|     autoindex " << ((it->second.get_autoindex() == AUTOINDEX_ON) ? "on" : "off") << std::endl;
            if (it->second.get_max_body_size() != -1) s << "|     max_body_size " << it->second.get_max_body_size() << std::endl;
            if(!(it->second.get_indexes().empty())){
                s << "|     Index [";
                std::vector<std::string>::iterator i_it(it->second.get_indexes().begin());
                for (; i_it != it->second.get_indexes().end() ; ++i_it)
                    s << *i_it << " ";
                s << "]" << std::endl;
            }
            if(!(it->second.get_limit_except().empty())){
                s << "|     Methods [";
                std::vector<std::string>::iterator m_it(it->second.get_limit_except().begin());
                for (; m_it != it->second.get_limit_except().end() ; ++m_it)
                    s << *m_it << " ";
                s << "]" << std::endl;
            }
            if(!(it->second.get_cgi().empty())) s << "|     CGI " << it->second.get_cgi() << std::endl;
            s << "|     ]" << std::endl;
        }
    }
    s << "|**********************************************|"<< std::endl;
    return (s);
}

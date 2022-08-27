/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmeira <fmeira@student.42lisboa.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/18 18:13:55 by fmeira            #+#    #+#             */
/*   Updated: 2022/08/27 21:24:50 by fmeira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserver.hpp"
#include "ServerConfig.hpp"
#include "ConfigParser.hpp"

// Constructors
BaseConfig::BaseConfig() : _autoindex(AUTOINDEX_UNSET), _client_max_body_size(-1)
{};
LocationConfig::LocationConfig()
{};
ServerConfig::ServerConfig()
{
    Listen  init_listen;

    init_listen.ip = "127.0.0.1";
    init_listen.port = 8080;
    init_listen.is_set = false;
    this->_listens.push_back(init_listen);
};

// Destructors
BaseConfig::~BaseConfig()
{};
LocationConfig::~LocationConfig()
{};
ServerConfig::~ServerConfig()
{};

// Copy constructor
ServerConfig::ServerConfig(const ServerConfig& param)
{
    this->_root = param._root;
    this->_autoindex = param._autoindex;
    this->_error_pages = param._error_pages;
    this->_client_max_body_size = param._client_max_body_size;
    this->_indexes = param._indexes;
    this->_locations = param._locations;
    this->_listens = param._listens;
    this->_server_names = param._server_names;
}

LocationConfig::LocationConfig(const LocationConfig& param)
{
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

    bool valid_method(std::string str)
    {
        return (str == "GET" || str == "POST" || str == "DELETE");}

    bool is_number(const std::string& str)
    {
        return (!str.empty() && (str.find_first_not_of("[0123456789]") == std::string::npos));
    }

    std::vector<std::string> split(const std::string& str, char delim)
    {
        size_t                      i = 0;
        std::vector<std::string>    vec;
        size_t                      pos = str.find(delim);

        while (pos != std::string::npos)
        {
            vec.push_back(str.substr(i, pos - i));
            i = ++pos;
            pos = str.find(delim, pos);
        }
        vec.push_back(str.substr(i, str.length()));
        return (vec);
    }

    bool valid_ip(std::string ip)
    {
        if (ip == "0.0.0.0" || ip == "localhost" || ip == "*")
            return (true);

        std::vector<std::string> ip_vec = split(ip, '.');

        if (ip_vec.size() != 4)
            return (false);

        int                                      i;
        std::stringstream                        stoi;
        std::vector<std::string>::iterator it    = ip_vec.begin();
        for (; it != ip_vec.end(); it++)
        {
                stoi.clear();
                stoi << *it;
                stoi >> i;
            if (!is_number(*it) || i < 0 || i > 255)
                return (false);
        }
        return (true);
    }

    bool is_number(const std::string& s)
    {
        std::string::const_iterator it = s.begin();
        while (it != s.end() && std::isdigit(*it)) ++it;
        return !s.empty() && it == s.end();
    }
}
// ~~~~~~~~~~~~~~~~~~~~~~~~~~BaseConfig methods~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// BaseConfig Setters
// void BaseConfig::set_directive(int directive, std::string& content)

void    BaseConfig::set_root(bool has_separators, std::string &content)
{
    if (has_separators)
	    throw (MultipleArgumentsError(content));
    else if (!is_directory(content))
	    throw (BadDirectoryError(content));
    this->_root = content;
}

void    BaseConfig::set_autoindex(std::string &content)
{
    if (content == "on")
        this->_autoindex = AUTOINDEX_ON;
    else if (content == "off")
        this->_autoindex = AUTOINDEX_OFF;
    else
	    throw (ConfigurationDirectiveError(content));
}

void    BaseConfig::set_error_pages(std::string &content)
{
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

void    BaseConfig::set_max_body_size(bool has_separators,std::string &content)
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

void    BaseConfig::set_indexes(std::string &content)
{
    std::string tmp(content);
    char*       token = strtok(const_cast<char *>(tmp.c_str()), SEPARATORS);

    while (token != NULL)
    {
        std::string tokstr = std::string(token);
        if (!(is_file(tokstr)))
	        throw (BadFileError(tokstr));
        this->_indexes.push_back(tokstr);
        token = strtok(NULL, SEPARATORS);
    }
}

void    BaseConfig::set_redirect(bool has_separators, std::string &content)
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
int                             BaseConfig::get_max_body_size( void ) const {return (this->_client_max_body_size);}
const std::vector<std::string>& BaseConfig::get_indexes( void ) const{return (this->_indexes);}
const std::vector<Redirect>&    BaseConfig::get_redirect( void ){return (this->_redirect);}



//~~~~~~~~~~~~~~~~~~~~~~~~LocationConfig methods~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

bool    LocationConfig::is_empty( void )
{
    return (this->_root.empty() && this->_autoindex == AUTOINDEX_UNSET
        && this->_error_pages.empty() && this->_client_max_body_size == -1
        && this->_indexes.empty() && this->_redirect.empty()
        && this->_limit_except.empty() && this->_cgi.empty());
}

int     LocationConfig::find_directive(std::string &directive)
{
    const std::string   valid_location_directives[CONTEXT_DIRECTIVES] =
    {"root", "autoindex", "error_page", "client_max_body_size", "index",
    "redirect", "cgi", "limit_except"};

    int                 i = -1;

    while (++i < CONTEXT_DIRECTIVES)
        if (directive == valid_location_directives[i])
        return (i);
    throw (ConfigurationDirectiveError(directive));
    return (0);
}


// LocationConfig setters

void LocationConfig::set_directive(int directive, std::string& content)
{
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
        case DIRECTIVE_REDIRECT:
            this->set_redirect(has_separators, content);
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
    else if (!is_file(content))
	    throw (BadFileError(content));
    this->_cgi = content;
}

void    LocationConfig::set_limit_except(std::string &content)
{
    char *token = std::strtok(const_cast<char *>(content.c_str()), " ");

    while (token)
    {
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
const std::string&                LocationConfig::get_cgi( void ) const {return (this->_cgi);}
const std::vector<std::string>&   LocationConfig::get_limit_except( void ) const {return (this->_limit_except);}


//~~~~~~~~~~~~~~~~~~~~~~~~ServerConfig methods~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// ServerConfig utils
bool    ServerConfig::is_empty( void )
{
    return (this->_root.empty() && this->_autoindex == AUTOINDEX_UNSET
        && this->_error_pages.empty() && this->_client_max_body_size == -1
        && this->_indexes.empty() && this->_redirect.empty()
        && this->_listens[0].ip == "127.0.0.1" && this->_listens[0].port == 8080
        && this->_server_names.empty() && this->_locations.empty());
}

int     ServerConfig::find_directive(std::string &directive)
{
    const std::string   valid_server_directives[CONTEXT_DIRECTIVES] =
    {"root","autoindex", "error_page", "client_max_body_size", "index",
    "redirect", "listen","server_name"};

    int                 i = -1;

    while (++i < CONTEXT_DIRECTIVES)
        if (directive == valid_server_directives[i])
            return (i);
    throw (ConfigurationDirectiveError(directive));
    return (0);
}

// ServerConfig setters
void ServerConfig::set_directive(int directive, std::string& content)
{
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
        case DIRECTIVE_REDIRECT:
            this->set_redirect(has_separators, content);
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

void    ServerConfig::set_listen(bool has_separators, std::string &content)
{
    if (has_separators)
	    throw (MultipleArgumentsError(content));

    std::stringstream   stoi;
    Listen              new_listen;

    if (content.find(':') != std::string::npos)
    {
        std::string ip_str = content.substr(0, content.find(':'));
        std::string port_str = content.substr(content.find(':') + 1);
        if (!valid_ip(ip_str) || port_str.empty())
	        throw (ConfigurationDirectiveError(content));
        else if (ip_str == "*")
            new_listen.ip = "0.0.0.0";
        else if (ip_str == "localhost")
            new_listen.ip = "127.0.0.1";
        else
            new_listen.ip = ip_str;
        stoi << port_str;
        stoi >> new_listen.port;
    }
    else if (valid_ip(content))
    {
        if (content == "*")
            new_listen.ip = "0.0.0.0";
        else if (content == "localhost")
            new_listen.ip = "127.0.0.1";
        else
            new_listen.ip = content;
        new_listen.port = 8080;
    }
    else
    {
        stoi << content;
        stoi >> new_listen.port;
        if (new_listen.port > PORT_MAX || new_listen.port <= PORT_MIN)
	        throw (ConfigurationDirectiveError(content));
        new_listen.ip = "127.0.0.1";
    }
    if (this->_listens[0].is_set == false)
    {
        this->_listens[0].ip = new_listen.ip;
        this->_listens[0].port = new_listen.port;
        this->_listens[0].is_set = true;
    }
    else
        this->_listens.push_back(new_listen);
}

void    ServerConfig::set_server_name(std::string &content)
{
    std::string tmp(content);
    char *token = strtok(const_cast<char *>(tmp.c_str()), SEPARATORS);

    while (token != NULL)
    {
        this->_server_names.push_back(std::string(token));
        token = strtok(NULL, SEPARATORS);
    }
}

// ServerConfig Getters

const std::vector<Listen>&      ServerConfig::get_listens( void ) const {return (this->_listens);}
Locations&                      ServerConfig::get_locations( void ) {return (this->_locations);}
const std::vector<std::string>& ServerConfig::get_server_names( void ) const {return (this->_server_names);}


// Operators
ServerConfig& ServerConfig::operator= (const ServerConfig& param)
{
    if (this == &param)
        return (*this);

    this->_root = param._root;
    this->_autoindex = param._autoindex;
    this->_error_pages = param._error_pages;
    this->_client_max_body_size = param._client_max_body_size;
    this->_indexes = param._indexes;
    this->_locations = param._locations;
    this->_listens = param._listens;
    this->_server_names = param._server_names;

    return (*this);
}

LocationConfig& LocationConfig::operator= (const LocationConfig& param)
{
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

std::ostream& operator<<(std::ostream & s, ServerConfig & server) {
    s << "|**********************************************|"<< std::endl;
    s << "| Server";
    s << "{" << std::endl;
    if (!(server.get_listens().empty()) && server.get_listens()[0].is_set == true)
    {
        s << "| - listens [" << std::endl;
        std::vector<Listen> l_list(server.get_listens());
        std::vector<Listen>::iterator l_it(l_list.begin());
        for (; l_it != l_list.end() ; ++l_it)
            s << "|     " << l_it->ip << ":" << l_it->port << std::endl;
        s << "]" << std::endl;
    }
    if (!(server.get_root().empty()))  s << "| - root path " << server.get_root() << std::endl;
    if (!(server.get_error_pages().empty()))
    {
        s << "| - errors map [" << std::endl;
        std::map<std::string, std::vector<unsigned short> >::const_iterator it_m = server.get_error_pages().begin();
        std::vector<unsigned short>::const_iterator it_v;
        for (; it_m != server.get_error_pages().end() ; it_m++) {
            s << "|     " << it_m->first << ": [";
            for (it_v = (it_m->second).begin() ; it_v != it_m->second.end() ; it_v++)
                s << " " << *it_v << " ";
            s << "]" << std::endl;
        }
        std::cout << "|   ]" << std::endl;
    }
    if (!(server.get_server_names().empty()))
    {
        s << "| - server names [";
        std::vector<std::string> s_list(server.get_server_names());
        std::vector<std::string>::iterator s_it(s_list.begin());
        for (; s_it != s_list.end() ; ++s_it)
            s << " " << *s_it << " ";
        s << "]" << std::endl;
    }
    if (server.get_autoindex() != AUTOINDEX_UNSET) s << "| - autoindex " << (server.get_autoindex() == AUTOINDEX_ON ? "on" : "off") << std::endl;
    if(!(server.get_indexes().empty()))
    {
                s << "|      Index [";
                std::vector<std::string>::const_iterator i_it(server.get_indexes().begin());
                for (; i_it != server.get_indexes().end() ; ++i_it)
                    s << *i_it;
                s << "]" << std::endl;
            }
    if (server.get_max_body_size() != -1) s << "| - max_body_size " << server.get_max_body_size() << std::endl;
    if (!(server.get_locations().empty()))
    {
        s << "| - [" << server.get_locations().size() << "] locations {" << std::endl;
        Locations list = server.get_locations();
        Locations::iterator it(list.begin());
        for (; it != list.end() ; ++it) {
            s << "|   -location for " << it->first << "{ \n";
            if (!(it->second.get_root().empty())) s << "|      root path " << it->second.get_root() << std::endl;
            if (!(server.get_error_pages().empty()))
            {
                s << "|      errors map [" << std::endl;
                std::map<std::string, std::vector<unsigned short> >::const_iterator it_m = it->second.get_error_pages().begin();
                std::vector<unsigned short>::const_iterator it_v;
                for (; it_m !=  it->second.get_error_pages().end() ; it_m++) {
                    s << "|        " << it_m->first << ": [";
                    for (it_v = (it_m->second).begin() ; it_v != it_m->second.end() ; it_v++)
                        s << " " << *it_v << " ";
                    s << "]" << std::endl;
                }
                std::cout << "|      ]" << std::endl;
            }
            if (it->second.get_autoindex() != AUTOINDEX_UNSET)s << "|      autoindex " << ((it->second.get_autoindex() == AUTOINDEX_ON) ? "on" : "off") << std::endl;
            if (it->second.get_max_body_size() != -1) s << "|      max_body_size " << it->second.get_max_body_size() << std::endl;
            if(!(it->second.get_indexes().empty()))
            {
                s << "|      Index [";
                std::vector<std::string>::const_iterator i_it(it->second.get_indexes().begin());
                for (; i_it != it->second.get_indexes().end() ; ++i_it)
                    s << *i_it;
                s << "]" << std::endl;
            }
            if(!(it->second.get_redirect().empty()))
            {
                s << "|      Redirect [";
                std::vector<Redirect>::const_iterator r_it(it->second.get_redirect().begin());
                for (; r_it != it->second.get_redirect().end() ; ++r_it)
                    s << " " << "Path = " << r_it->new_path << ", Code = " << r_it->code << " ";
                s << "]" << std::endl;
            }
            if(!(it->second.get_limit_except().empty()))
            {
                s << "|      Accepted methods [";
                std::vector<std::string>::const_iterator m_it(it->second.get_limit_except().begin());
                for (; m_it != it->second.get_limit_except().end() ; ++m_it)
                    s << " " << *m_it << " ";
                s << "]" << std::endl;
            }
            if(!(it->second.get_cgi().empty())) s << "|      CGI " << it->second.get_cgi() << std::endl;
            s << "|    }" << std::endl;
        }
        s << "|   }" << std::endl;
    }
    s << "| }\n";
    s << "|**********************************************|"<< std::endl;
    return (s);
}

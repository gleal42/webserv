/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/18 18:13:55 by fmeira            #+#    #+#             */
/*   Updated: 2022/09/07 15:55:30 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "ServerConfig.hpp"
// #include "BaseConfig.hpp"
// #include "LocationConfig.hpp"
#include "webserver.hpp"

// Constructors
ServerConfig::ServerConfig()
{
    Listen  init_listen;

    init_listen.ip = "0.0.0.0";
    init_listen.port = 80;
    init_listen.is_set = false;
    this->_listens.push_back(init_listen);
};

// Destructors
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

// ServerConfig utils
namespace {
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
}

//~~~~~~~~~~~~~~~~~~~~~~~~ServerConfig methods~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// ServerConfig utils
bool    ServerConfig::is_empty( void )
{
    return (this->_root.empty() && this->_autoindex == AUTOINDEX_UNSET
        && this->_error_pages.empty() && this->_client_max_body_size == -1
        && this->_indexes.empty() && this->_redirect.empty()
        && this->_listens[0].ip == "0.0.0.0" && this->_listens[0].port == 80
        && this->_server_names.empty() && this->_locations.empty());
}

int     ServerConfig::find_directive(const std::string &directive)
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
void ServerConfig::set_directive(int directive, const std::string& content)
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

void    ServerConfig::set_listen(bool has_separators, const std::string &content)
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

void    ServerConfig::set_server_name( const std::string &content )
{
    std::string tmp(content);
    char *token = strtok(const_cast<char *>(tmp.c_str()), SEPARATORS);

    while (token != NULL)
    {
        this->_server_names.push_back(std::string(token));
        token = strtok(NULL, SEPARATORS);
    }
}

void    ServerConfig::set_location(const std::string &name, const LocationConfig& location)
{
    _locations[name] = location;
}

// ServerConfig Getters

const std::vector<Listen>&      ServerConfig::get_listens( void ) const {return (this->_listens);}
const Locations&                      ServerConfig::get_locations( void ) const {return (this->_locations);}
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

const std::string &     ServerConfig::get_ip( void ) const
{
    return _listens[0].ip;
}

int                     ServerConfig::get_port( void ) const
{
    return _listens[0].port;
}

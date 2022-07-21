/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmeira <fmeira@student.42lisboa.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 00:50:04 by fmeira            #+#    #+#             */
/*   Updated: 2022/07/22 00:51:12 by fmeira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __CONFIG_PARSER_H__
# define __CONFIG_PARSER_H__

# include "ServerConfig.hpp"
# include <macros.hpp>


# include <fstream>
# include <cstring>
# include <sstream>
# include <iomanip>
# include <iostream>
# include <vector>
# include <map>
# include <algorithm>
# include <sys/stat.h>
# include <string.h>


# define SEPARATORS " \t\v\n\r\f"
# define CONTEXT_DIRECTIVES 8
# define SERVER_CONTEXT     1
# define LOCATION_CONTEXT   2

# define ROOT				1
# define AUTOINDEX			2
# define ERRORPAGE			3
# define MAXBODYSIZE    	4
# define INDEX				5
# define LISTEN				6
# define SERVERNAME			7
# define LOCATION			8
# define ALLOWEDMETHODS		9
# define CGI				10
# define CGIBIN				11

// ************************************************************************** //
//                               ConfigParser Class                             	  //
// ************************************************************************** //

/*
A class to parse the config_file and turn it into ServerConfig objects

Needs to be able to:

	- Return errors if not valid
	- Create as many ServerConfigs as `server` blocks in the config_file

*/
class ConfigParser
{

public:
    struct ConfigurationFileError : public std::runtime_error{
        ConfigurationFileError(void);
    };
    struct ConfigurationDirectiveError : public std::runtime_error{
        ConfigurationDirectiveError(const std::string err);
    };
    struct ConfigurationSyntaxError : public std::runtime_error{
        ConfigurationSyntaxError(void);
    };
    struct LocationPathError : public std::runtime_error{
        LocationPathError(const std::string err);
    };
    struct NestedContextError : public std::runtime_error{
        NestedContextError(void);
    };
    struct EmptyContextBlockError : public std::runtime_error{
        EmptyContextBlockError(void);
    };
    struct DirectiveOutOfScopeError : public std::runtime_error{
        DirectiveOutOfScopeError(const std::string err);
    };
    struct OpenContextBlockError : public std::runtime_error{
        OpenContextBlockError(void);
    };
    struct LocationURIError : public std::runtime_error{
        LocationURIError(void);
    };
    struct BadDirectoryError : public std::runtime_error{
        BadDirectoryError(const std::string err);
    };

    ConfigParser(char *str) : _config_file(str){};
    ~ConfigParser(void){};
    ConfigParser &operator=(ConfigParser const &rhs);

    // Methods
    ServerConfig    config(int const index) const;
    int             configs_amount(void) const;
    void            call(void);
    bool            server_is_empty(ServerConfig& target);
    bool            location_is_empty(LocationConfig& target);
    template <typename T>
    void            set_directive(int directive, std::string& content, T* new_object);
    void            context_parser(std::ifstream *file, int context, std::string location_path = "", ServerConfig* server_ptr = 0);

    // Config vector
    Configs         server_configs;

private:
    ConfigParser(void);
    ConfigParser(ConfigParser const &src);

    // Setters
    template <typename T>
    void            set_root(bool has_separators, std::string &content, T *new_object);
    template <typename T>
    void            set_autoindex(std::string &content, T *new_object);
    template <typename T>
    void            set_error_pages(std::string &content, T *new_object);
    template <typename T>
    void            set_max_body_size(int has_separators,std::string &content, T *new_object);
    template <typename T>
    void            set_indexes(std::string &content, T *new_object);
    void            set_cgi_bin(std::string &content, LocationConfig *new_location);
    void            set_cgi(std::string &content, LocationConfig *new_location);
    void            set_allowed_methods(std::string &content, LocationConfig &new_object);
    void            set_ip(std::string &content, ServerConfig &new_object);
    void            set_port(std::string &content, ServerConfig &new_object);
    void            set_locations(std::string &content, ServerConfig &new_object);
    void            set_server_name(bool has_separators, std::string &content, ServerConfig &new_object);

    char            *_config_file;
};
#endif /* __CONFIG_PARSER_H__ */

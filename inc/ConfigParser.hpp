/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmeira <fmeira@student.42lisboa.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 00:50:04 by fmeira            #+#    #+#             */
/*   Updated: 2022/08/12 14:45:41 by fmeira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __CONFIG_PARSER_H__
# define __CONFIG_PARSER_H__

# include "ServerConfig.hpp"
# include "webserver.hpp"
# include <macros.hpp>


# include <fstream>
# include <cstring>
# include <sstream>
# include <iomanip>
# include <iostream>
# include <vector>
# include <map>
# include <algorithm>
# include <string.h>


# define SEPARATORS         " \t\v\n\r\f"
# define CONTEXT_DIRECTIVES 7
# define SERVER_CONTEXT     1
# define LOCATION_CONTEXT   2

enum directives{
    DIRECTIVE_ROOT,
    DIRECTIVE_AUTOINDEX,
    DIRECTIVE_ERRORPAGE,
    DIRECTIVE_MAXBODYSIZE,
    DIRECTIVE_INDEX,
    DIRECTIVE_CGI = 5,
    DIRECTIVE_LIMITEXCEPT = 6,
    DIRECTIVE_LISTEN = 5,
    DIRECTIVE_SERVERNAME = 6,
};

// ************************************************************************** //
//                               ConfigParser Class                             	  //
// ************************************************************************** //

/*
A class to parse the config_file and turn it into ServerConfig objects

Needs to be able to:

	- Return errors if not valid
	- Create as many ServerConfigs as `server` blocks in the config_file

*/

struct ConfigError : virtual public std::runtime_error{
        ConfigError();
        virtual ~ConfigError (void) throw(){}
};

struct ConfigurationFileError : public ConfigError{
    ConfigurationFileError(void);
};
struct ConfigurationDirectiveError : public ConfigError{
    ConfigurationDirectiveError(const std::string err);
};
struct ConfigurationSyntaxError : public ConfigError{
    ConfigurationSyntaxError(void);
};
struct LocationPathError : public ConfigError{
    LocationPathError(const std::string err);
};
struct NestedContextError : public ConfigError{
    NestedContextError(void);
};
struct EmptyContextBlockError : public ConfigError{
    EmptyContextBlockError(void);
};
struct DirectiveOutOfScopeError : public ConfigError{
    DirectiveOutOfScopeError(const std::string err);
};
struct OpenContextBlockError : public ConfigError{
    OpenContextBlockError(void);
};
struct LocationURIError : public ConfigError{
    LocationURIError(void);
};
struct BadDirectoryError : public ConfigError{
    BadDirectoryError(const std::string err);
};
struct MultipleArgumentsError : public ConfigError{
    MultipleArgumentsError(const std::string err);
};


class ConfigParser
{
    public:
        ConfigParser(std::string);
        ~ConfigParser(void);
        ConfigParser(ConfigParser const &src);

        // Methods
        // ServerConfig    config(int const index) const;
        int             configs_amount(void) const;
        const Configs   call(void);
        void            context_parser(std::ifstream *file, int context, std::string location_path = "", ServerConfig* server_ptr = 0);
        // LocationConfig  location_context_parser(std::ifstream *file, std::string location_path);
        // void            server_context_parser(std::ifstream *file);

        // Config vector
        Configs         server_configs;

    private:
        ConfigParser(void);
        std::string     _config_file;
};
#endif /* __CONFIG_PARSER_H__ */

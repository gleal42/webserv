/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 00:49:53 by fmeira            #+#    #+#             */
/*   Updated: 2022/08/10 00:26:47 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Exceptions~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

ConfigError::ConfigError()
: std::runtime_error("")
{
}

ConfigError::~ConfigError (void) throw()
{
}


ConfigurationFileError::ConfigurationFileError(void)
    : std::runtime_error("Error while opening configuration file")
{ /* No-op */}

ConfigurationDirectiveError::ConfigurationDirectiveError(const std::string err)
    : std::runtime_error("Error: " + err + " directive is invalid")
{ /* No-op */}

ConfigurationSyntaxError::ConfigurationSyntaxError()
    : std::runtime_error("Error. Invalid syntax on the configuration file")
{ /* No-op */}

LocationPathError::LocationPathError(const std::string err)
    : std::runtime_error("Error. The following path isn't accessible: " + err)
{ /* No-op */}

NestedContextError::NestedContextError(void)
    : std::runtime_error("Error trying to nest a context block")
{ /* No-op */}

EmptyContextBlockError::EmptyContextBlockError(void)
    : std::runtime_error("Error trying to add an empty context block")
{ /* No-op */}

DirectiveOutOfScopeError::DirectiveOutOfScopeError(const std::string err)
    : std::runtime_error("Error: " + err + ". All declarations must be inside server scopes")
{ /* No-op */}

OpenContextBlockError::OpenContextBlockError(void)
    : std::runtime_error("Error: missing closing bracket (}) on context")
{ /* No-op */}

LocationURIError::LocationURIError(void)
    : std::runtime_error("Error: missing location URI")
{ /* No-op */}

BadDirectoryError::BadDirectoryError(const std::string err)
    : std::runtime_error("Error: " + err + ". Please provide a valid directory")
{ /* No-op */}

MultipleArgumentsError::MultipleArgumentsError(const std::string err)
    : std::runtime_error("Error: " + err + ". This directive can only have one argument")
{ /* No-op */}


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Constructors~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
ConfigParser::ConfigParser(std::string config_file) : _config_file(config_file){ /* no-op */ }
ConfigParser::ConfigParser( ConfigParser const & src ) { *this = src; }

ConfigParser::~ConfigParser(void)
{
	
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~Config-parsing utils~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

namespace {
    const std::string valid_server_directives[CONTEXT_DIRECTIVES] =
    {"root","autoindex", "error_page", "client_max_body_size", "index",
    "listen","server_name"};

    const std::string valid_location_directives[CONTEXT_DIRECTIVES] =
    {"root", "autoindex", "error_page", "client_max_body_size", "index",
    "limit_except","cgi"};

    std::string &strtrim(std::string &str)
    {
        str.erase(str.find_last_not_of(SEPARATORS) + 1);
        str.erase(0, str.find_first_not_of(SEPARATORS));
        return (str);
    };

    // Não se pode usar este numero se depois switch case
    // vão usar um enum comum
    int find_directive(std::string &directive, int context)
    {
        const std::string   *valid_directives;
        int                 i = 0;

        if (context == LOCATION_CONTEXT)
            valid_directives = valid_location_directives;
        else if (context == SERVER_CONTEXT)
            valid_directives = valid_server_directives;

        while (i < CONTEXT_DIRECTIVES)
            if (directive == valid_directives[++i])
                return (i);

        throw (ConfigurationDirectiveError(directive));
        return (0);
    }
}

/*~~~~~~~~~~~~~~~~~~~~~~~ConfigParser member functions~~~~~~~~~~~~~~~~~~~~~~~~*/

ServerConfig   ConfigParser::config( int const index ) const { return server_configs[index]; }

int    ConfigParser::configs_amount( void ) const { return server_configs.size(); }

// This function parses a context block.
// There are two possible contexts:
//    - Server context (Inside a server Scope) [server {...}]
//    - Location context (inside Location scope, which must always be inside a server Scope) [server {... location {...}}]
// It will parse all the directives inside the context block, line by line, one directive per line
void    ConfigParser::context_parser(std::ifstream *file, int context, std::string location_path, ServerConfig* server_ptr)
{
    std::string     line;
    std::string     directive;
    std::string     content;
    ServerConfig    new_server;
    LocationConfig  new_location;
    size_t          separator;

    while (std::getline(*file, line))
    {
        line = strtrim(line);
        if (!line.length() || line[0] == '#')
            continue;
        if (line == "}"){
            if (context == SERVER_CONTEXT){
                if (new_server.is_empty())
                    throw (EmptyContextBlockError());
                this->server_configs.push_back(new_server);
            }
            else if (context == LOCATION_CONTEXT){
                if (new_location.is_empty())
                    throw (EmptyContextBlockError());
                if (is_directory(location_path))
                    server_ptr->get_locations()[location_path] = new_location;
            }
            return;
        }
        separator = line.find_first_of(SEPARATORS);
        directive = line.substr(0, separator);
        content = line.substr(separator + 1);
        std::cout << std::left << (context == LOCATION_CONTEXT ? "Location" : "Server") << " context: " << "[" << directive << "]" << " - [" << content << "]\n";
        if (directive == "server")
            throw (NestedContextError());
        else if (directive == "location"){
            separator = content.find_first_of("{");
            if (separator == std::string::npos)
                throw (ConfigurationSyntaxError());
            else if (separator < 2)
                throw (LocationURIError());
            else if (context == LOCATION_CONTEXT)
                throw (NestedContextError());
            content = content.substr(0, separator - 1);
            context_parser(file, LOCATION_CONTEXT, content, &new_server);
        }
        else{
            if (content[content.length() - 1] != ';')
                throw (ConfigurationSyntaxError());
            content = content.substr(0, content.length() - 1);
            if (context == SERVER_CONTEXT)
                new_server.set_directive(find_directive(directive, context), content);
            else if (context == LOCATION_CONTEXT)
                new_location.set_directive(find_directive(directive, context), content);
        }
    }
    throw (OpenContextBlockError());
};

// This function opens the _config_file, and then parses the file while looking for an opening server block (aka "server {")
// Then, calls context_parser() to parse the server block
// OBS: So far, the new ServerConfig object is being stored inside the ConfigParser
void    ConfigParser::call()
{
    std::ifstream   file;
    std::string     line;
    std::string     directive;
    size_t          separator;

    try{
        file.open(this->_config_file.c_str());
        if (!file.is_open())
            throw ConfigurationFileError();
        while (std::getline(file, line))
        {
            line = strtrim(line);
            if (!line.length() || line[0] == '#')
                continue;
            separator = line.find_first_of(SEPARATORS);
            directive = line.substr(0, separator);
            if (directive == "server"){
                if (line[separator + 1] != '{')
                    throw ConfigurationSyntaxError();
                context_parser(&file, SERVER_CONTEXT);
            }
            else
                throw DirectiveOutOfScopeError(directive);
        }
    }
    catch (ConfigError &e) {
		    ERROR(e.what());
    }
    file.close();
};

// int main(int ac, char **av)
// {
//     std::string file(av[1]);
//     if (ac == 2)
//     {
//         ConfigParser config_parser(file);
//         std::cout << "error page inside config is " << config_parser.server_configs[0].get_error_pages().begin()->first;
//         std::cout << "\nerror page inside location config is " << config_parser.server_configs[0].get_locations()["/home/user/Desktop/git/webserv/test"].get_error_pages().begin()->first;
//     }

//     return (0);
// };


void	ConfigParser::set_general_conf(void)
{
    ServerConfig general_server;
    general_server.set_listen(0, "8080");
    general_server.set_server_name("boss_group");
    general_server.set_indexes("index.html");
    LocationConfig default_location;
    default_location.set_limit_except("GET POST PUT DELETE");
    general_server.set_location("/", default_location);
    server_configs.push_back(general_server);
}

void	ConfigParser::set_tester_conf(void)
{
    ServerConfig general_server;
    general_server.set_listen(0, "8080");
    general_server.set_server_name("boss_group");
    general_server.set_indexes("index.html");

    server_configs.push_back(general_server);
}
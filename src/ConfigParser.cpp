/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmeira <fmeira@student.42lisboa.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 00:49:53 by fmeira            #+#    #+#             */
/*   Updated: 2022/08/12 14:36:14 by fmeira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Exceptions~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

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
ConfigError::ConfigError() : std::runtime_error("Error in configuration"){};
ConfigParser::ConfigParser(std::string config_file) : _config_file(config_file){ /* no-op */ }
ConfigParser::ConfigParser( ConfigParser const & src ) { *this = src; }

ConfigParser::~ConfigParser(){};


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~Config-parsing utils~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

namespace {
    std::string &strtrim(std::string &str)
    {
        str.erase(str.find_last_not_of(SEPARATORS) + 1);
        str.erase(0, str.find_first_not_of(SEPARATORS));
        return (str);
    };
}

/*~~~~~~~~~~~~~~~~~~~~~~~ConfigParser member functions~~~~~~~~~~~~~~~~~~~~~~~~*/

// const ServerConfig   ConfigParser::config( int const index ) const { return (server_configs[index]); }

int    ConfigParser::configs_amount( void ) const { return (server_configs.size()); }

// This function parses a context block.
// There are two possible contexts:
//    - Server context (Inside a server Scope) [server {...}]
//    - Location context (inside Location scope, which must always be inside a server Scope) [server {... location {...}}]
// It will parse all the directives inside the context block, line by line, one directive per line
// void    ConfigParser::context_parser(std::ifstream *file, int context, std::string location_path, ServerConfig* server_ptr)
// {
//     std::string     line;
//     std::string     directive;
//     std::string     content;
//     ServerConfig    new_server;
//     LocationConfig  new_location;
//     size_t          separator;

//     while (std::getline(*file, line))
//     {
//         line = strtrim(line);
//         if (!line.length() || line[0] == '#')
//             continue;
//         std::cout << "line is: " << line << "\n";
//         if (line == "}"){
//             if (context == SERVER_CONTEXT){
//                 if (new_server.is_empty())
//                     throw (EmptyContextBlockError());
//                 this->server_configs.push_back(new_server);
//                 std::cout << "errorpage no new_server: " << new_server.get_error_pages()["/home/user/Desktop/git"][0] << "\n";
//                 std::cout << "location.max_body_size no new_server: " << new_server.get_locations()[location_path].get_max_body_size() << "\n";
//             }
//             else if (context == LOCATION_CONTEXT){
//                 if (new_location.is_empty())
//                     throw (EmptyContextBlockError());
//                 if (server_ptr->is_empty()) std::cout << "server empty\n";
//                 if (!(is_directory(location_path)))
// 	                throw (BadDirectoryError(content));
//                 std::cout << "size is: " << server_ptr->get_locations().size() << "\n";
//                 std::cout << "new_location.max_body_size a entrar no server_ptr: " << new_location.get_max_body_size() << "\n";
//                 server_ptr->get_locations()[location_path] = new_location;
//                 std::cout << "size is: " << server_ptr->get_locations().size() << "\n";
//                 std::cout << "errorpage no server_ptr: " << server_ptr->get_error_pages()["/home/user/Desktop/git"][0] << "\n";
//                 std::cout << "location.max_body_size no server_ptr: " << server_ptr->get_locations()[location_path].get_max_body_size() << "\n";

//             }
//             return;
//         }
//         separator = line.find_first_of(SEPARATORS);
//         directive = line.substr(0, separator);
//         content = line.substr(separator + 1);
//         // std::cout << std::left << (context == LOCATION_CONTEXT ? "Location" : "Server") << " context: " << "[" << directive << "]" << " - [" << content << "]\n";
//         if (directive == "server")
//             throw (NestedContextError());
//         else if (directive == "location"){
//             separator = content.find_first_of("{");
//             if (separator == std::string::npos)
//                 throw (ConfigurationSyntaxError());
//             else if (separator < 2)
//                 throw (LocationURIError());
//             else if (context == LOCATION_CONTEXT)
//                 throw (NestedContextError());
//             content = content.substr(0, separator - 1);
//             context_parser(file, LOCATION_CONTEXT, content, &new_server);
//         }
//         else{
//             if (content[content.length() - 1] != ';'){
//                 throw (ConfigurationSyntaxError());
//             }
//             content = content.substr(0, content.length() - 1);
//             if (context == SERVER_CONTEXT)
//                 new_server.set_directive((new_server.find_directive(directive)), content);
//             else if (context == LOCATION_CONTEXT)
//                 new_location.set_directive((new_location.find_directive(directive)), content);
//             std::cout << "location.max_body_size saida do set_directive: " << new_location.get_max_body_size() << "\n";
//         }
//     }
//     throw (OpenContextBlockError());
// };

LocationConfig    ConfigParser::location_context_parser(std::ifstream *file, std::string location_path)
{
    std::string     line;
    std::string     directive;
    std::string     content;
    LocationConfig  new_location;
    size_t          separator;

    while (std::getline(*file, line))
    {
        line = strtrim(line);
        if (!line.length() || line[0] == '#')
            continue;
        std::cout << "line is: " << line << "\n";
        if (line == "}"){
            if (new_location.is_empty())
                throw (EmptyContextBlockError());
            if (!(is_directory(location_path)))
                throw (BadDirectoryError(content));
            return (new_location);
        }
        separator = line.find_first_of(SEPARATORS);
        directive = line.substr(0, separator);
        content = line.substr(separator + 1);
        if (directive == "server" || directive == "location")
            throw (NestedContextError());
        if (content[content.length() - 1] != ';'){
            throw (ConfigurationSyntaxError());
        }
        content = content.substr(0, content.length() - 1);
        new_location.set_directive((new_location.find_directive(directive)), content);
    }
    throw (OpenContextBlockError());
};

void    ConfigParser::server_context_parser(std::ifstream *file)
{
    std::string     line;
    std::string     directive;
    std::string     content;
    ServerConfig    new_server;
    size_t          separator;

    while (std::getline(*file, line))
    {
        line = strtrim(line);
        if (!line.length() || line[0] == '#')
            continue;
        std::cout << "line is: " << line << "\n";
        if (line == "}"){
            if (new_server.is_empty())
                throw (EmptyContextBlockError());
            this->server_configs.push_back(new_server);
            return;
        }
        separator = line.find_first_of(SEPARATORS);
        directive = line.substr(0, separator);
        content = line.substr(separator + 1);
        // std::cout << std::left << (context == LOCATION_CONTEXT ? "Location" : "Server") << " context: " << "[" << directive << "]" << " - [" << content << "]\n";
        if (directive == "server")
            throw (NestedContextError());
        else if (directive == "location"){
            separator = content.find_first_of("{");
            if (separator == std::string::npos)
                throw (ConfigurationSyntaxError());
            else if (separator < 2)
                throw (LocationURIError());
            content = content.substr(0, separator - 1);
            LocationConfig  new_location = location_context_parser(file, content);
            new_server.get_locations()[content] = new_location;
        std::cout << "location.max_body_size no new_server: " << new_server.get_locations()[content].get_max_body_size() << "\n";

        }
        else{
            if (content[content.length() - 1] != ';'){
                throw (ConfigurationSyntaxError());
            }
            content = content.substr(0, content.length() - 1);
            new_server.set_directive((new_server.find_directive(directive)), content);
        }
    }
    throw (OpenContextBlockError());
};

// This function opens the _config_file, and then parses the file while looking for an opening server block (aka "server {")
// Then, calls context_parser() to parse the server block
// OBS: So far, the new ServerConfig object is being stored inside the ConfigParser
const Configs ConfigParser::call()
{
    std::ifstream   file;
    std::string     line;
    std::string     directive;
    size_t          separator;

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
            server_context_parser(&file);
        }
        else
            throw DirectiveOutOfScopeError(directive);
    }
    file.close();
    std::cout << "saiu\n";
    std::cout << "location outside is " << server_configs[0].get_locations()["/home/user/Desktop"].get_max_body_size() << "\n";
    // std::vector<ServerConfig>::iterator it = server_configs.begin();
    // for(; it != server_configs.end(); it++)
    //     std::cout << *it;
    return (this->server_configs);
};

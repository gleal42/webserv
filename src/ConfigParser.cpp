/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmeira <fmeira@student.42lisboa.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 00:49:53 by fmeira            #+#    #+#             */
/*   Updated: 2022/07/27 20:28:46 by fmeira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Exceptions~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

ConfigParser::ConfigurationFileError::ConfigurationFileError(void)
    : std::runtime_error("Error while opening configuration file")
{ /* No-op */}

ConfigParser::ConfigurationDirectiveError::ConfigurationDirectiveError(const std::string err)
    : std::runtime_error("Error: " + err + " directive is invalid")
{ /* No-op */}

ConfigParser::ConfigurationSyntaxError::ConfigurationSyntaxError()
    : std::runtime_error("Error. Invalid syntax on the configuration file")
{ /* No-op */}

ConfigParser::LocationPathError::LocationPathError(const std::string err)
    : std::runtime_error("Error. The following path isn't accessible: " + err)
{ /* No-op */}

ConfigParser::NestedContextError::NestedContextError(void)
    : std::runtime_error("Error trying to nest a context block")
{ /* No-op */}

ConfigParser::EmptyContextBlockError::EmptyContextBlockError(void)
    : std::runtime_error("Error trying to add an empty context block")
{ /* No-op */}

ConfigParser::DirectiveOutOfScopeError::DirectiveOutOfScopeError(const std::string err)
    : std::runtime_error("Error: " + err + ". All declarations must be inside server scopes")
{ /* No-op */}
ConfigParser::OpenContextBlockError::OpenContextBlockError(void)
    : std::runtime_error("Error: missing closing bracket (}) on context")
{ /* No-op */}
ConfigParser::LocationURIError::LocationURIError(void)
    : std::runtime_error("Error: missing location URI")
{ /* No-op */}
ConfigParser::BadDirectoryError::BadDirectoryError(const std::string err)
    : std::runtime_error("Error: " + err + ". Please provide a valid directory")
{ /* No-op */}


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~Config-parsing utils~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

namespace {
    const std::string valid_server_directives[CONTEXT_DIRECTIVES] =
    {"root","autoindex", "error_page", "client_max_body_size", "index",
    "listen","server_name","location"};

    const std::string valid_location_directives[CONTEXT_DIRECTIVES] =
    {"root", "autoindex", "error_page", "client_max_body_size", "index",
    "limit_except", "cgi", "cgi-bin"};

    std::string &strtrim(std::string &str)
    {
        str.erase(str.find_last_not_of(SEPARATORS) + 1);
        str.erase(0, str.find_first_not_of(SEPARATORS));
        return (str);
    };

    int find_directive(std::string &directive, int context)
    {
        const std::string   *valid_directives;
        int                 i = 0;

        if (context == LOCATION_CONTEXT)
            valid_directives = valid_location_directives;
        else if (context == SERVER_CONTEXT)
            valid_directives = valid_server_directives;

        while (i < CONTEXT_DIRECTIVES)
            if (directive == valid_directives[i++])
                return (i);

        throw ConfigParser::ConfigurationDirectiveError(directive);
        return (0);
    }

    // bool is_directory(std::string &path)
    // {
    //     struct stat s;

    //     if (lstat(path.c_str(), &s) == 0)
    //         if (S_ISDIR(s.st_mode))
    //             return (true);
    //     return (false);
    // }

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
}


/*~~~~~~~~~~~~~~~~~~~~~~~ConfigParser member functions~~~~~~~~~~~~~~~~~~~~~~~~*/

ServerConfig   ConfigParser::config( int const index ) const { return server_configs[index]; }

int    ConfigParser::configs_amount( void ) const { return server_configs.size(); }

bool    ConfigParser::server_is_empty(ServerConfig& target)
{
    return (target.root.empty() && !target.autoindex
            && target.error_pages.empty() && !target.client_max_body_size
            && target.indexes.empty() && target.ip.empty() && !target.port
            && target.server_name.empty() && target.locations.empty());
}

bool    ConfigParser::location_is_empty(LocationConfig& target)
{
    return (target.root.empty() && !target.autoindex
            && target.error_pages.empty() && !target.client_max_body_size
            && target.indexes.empty() && target.limit_except.empty()
            && target.cgi_map.empty() && target.cgi_bin.empty());
}

// Setters

template <typename T>
void    ConfigParser::set_root(bool has_separators, std::string &content, T *new_object){
    if (has_separators)
        throw ConfigParser::ConfigurationDirectiveError(content);
    // else if (!is_directory(content))
    //     throw ConfigParser::BadDirectoryError(content);
    new_object->root = content;
}

template <typename T>
void    ConfigParser::set_autoindex(std::string &content, T* new_object){
    if (content == "on")
        new_object->autoindex = true;
    else if (content == "off")
        new_object->autoindex = false;
    else
        throw ConfigurationDirectiveError(content);
}

template <typename T>
void ConfigParser::set_error_pages(std::string &content, T *new_object){
    size_t found = content.find_last_of(SEPARATORS);
    if (found == std::string::npos)
        throw ConfigParser::ConfigurationDirectiveError(content);

    std::string         error_path = content.substr(found + 1);
    // if (!(is_directory(error_path)))
    //     throw ConfigParser::BadDirectoryError(error_path);

    std::string         codes = content.substr(0, found);
    std::stringstream   stoi_converter;
    int                 converted_number;
    char*               token = std::strtok(const_cast<char *>(codes.c_str()), SEPARATORS);
    size_t              i = 0;

    while (token != 0 && i++ < found){
        stoi_converter.clear();
        stoi_converter << token;
        stoi_converter >> converted_number;
        if (!valid_error_code(converted_number))
            throw ConfigParser::ConfigurationDirectiveError(std::string(token));
        new_object->error_pages[error_path].push_back(converted_number);
        token = std::strtok(NULL, SEPARATORS);
    }
}

template <typename T>
void    ConfigParser::set_max_body_size(int has_separators, std::string &content, T *new_object){
    if (has_separators)
        throw ConfigurationSyntaxError();
    int max_size;
    std::stringstream intValue(content);
    intValue >> max_size;
    if (max_size < 0)
        throw ConfigurationDirectiveError(content);
    new_object->_max_body_size = max_size;
}

template <typename T>
void ConfigParser::set_indexes(std::string &content, T *new_object){
    std::string tmp(content);
    char *token = strtok(const_cast<char *>(tmp.c_str()), SEPARATORS);
    while (token != NULL)
    {
        new_object->_indexes.push_back(token);
        token = strtok(NULL, SEPARATORS);
    }
}

// void ConfigParser::set_cgi(std::string &content, LocationConfig *new_location){

// }

// void ConfigParser::set_cgi_bin(std::string &content, LocationConfig *new_location){

// }

// void ConfigParser::set_limit_except(std::string &content, LocationConfig &new_object){

// }

// void ConfigParser::set_ip(std::string &content, ServerConfig &new_object){

// }

// void ConfigParser::set_port(std::string &content, ServerConfig &new_object){

// }

// void ConfigParser::set_locations(std::string &content, ServerConfig &new_object){

// }

// void ConfigParser::set_server_name(bool has_separators, std::string &content, ServerConfig &new_object){

// }


template <typename T>
void    ConfigParser::set_directive(int directive, std::string& content, T* new_object){
    bool    has_separators = (content.find(SEPARATORS) != std::string::npos);
    if (content.empty())
        throw ConfigurationSyntaxError();

    switch (directive)
    {
    case DIRECTIVE_ROOT:
        set_root(has_separators, content, new_object);
        break;
    case DIRECTIVE_AUTOINDEX:
        set_autoindex(content, new_object);
        break;
    case DIRECTIVE_ERRORPAGE:
        set_error_pages(content, new_object);
        break;
    case DIRECTIVE_MAXBODYSIZE:
        set_max_body_size(has_separators, content, new_object);
        break;
    case DIRECTIVE_INDEX:
        set_indexes(content, new_object);
        break;
    // case DIRECTIVE_CGI:
    //     set_cgi(content, new_object);
    //     break;
    // case DIRECTIVE_CGIBIN:
    //     set_cgi_bin(content, new_object);
    //     break;
    // case DIRECTIVE_ALLOWEDMETHODS:
    //     set_limit_except(content, new_object);
    //     break;
    // case DIRECTIVE_LISTEN:
    //     // set_c(content, new_object);
    //     break;
    // case DIRECTIVE_LOCATION:
    //     // set_cgi(content, new_object);
    //     break;
    // case DIRECTIVE_SERVERNAME:
    //     set_server_name(content, new_object);
    //     break;
    default:
        break;
    }
};

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
    ServerConfig    new_server = {};
    LocationConfig  new_location = {};
    size_t          separator;

    while (std::getline(*file, line))
    {
        line = strtrim(line);
        if (!line.length() || line[0] == '#')
            continue;
        if (line == "}"){
            if (context == SERVER_CONTEXT){
                if (server_is_empty(new_server))
                    throw ConfigParser::EmptyContextBlockError();
                this->server_configs.push_back(new_server);
            }
            else if (context == LOCATION_CONTEXT){
                if (location_is_empty(new_location))
                    throw ConfigParser::EmptyContextBlockError();
                server_ptr->locations[location_path] = new_location;
            }
            return;
        }
        separator = line.find_first_of(SEPARATORS);
        directive = line.substr(0, separator);
        content = line.substr(separator + 1);
        // std::cout << std::left << (context == LOCATION_CONTEXT ? "Location" : "Server") << " context: " << "[" << directive << "]" << " - [" << content << "]\n";
        if (directive == "server")
            throw ConfigParser::NestedContextError();
        else if (directive == "location"){
            separator = content.find_first_of("{");
            if (separator == std::string::npos)
                throw ConfigParser::ConfigurationSyntaxError();
            else if (separator < 2)
                throw ConfigParser::LocationURIError();
            else if (context == LOCATION_CONTEXT)
                throw ConfigParser::NestedContextError();
            content = content.substr(0, separator - 1);
            context_parser(file, LOCATION_CONTEXT, content, &new_server);
        }
        else{
            if (content[content.length() - 1] != ';')
                throw ConfigParser::ConfigurationSyntaxError();
            content = content.substr(0, content.length() - 1);
            if (context == SERVER_CONTEXT)
                set_directive(find_directive(directive, context), content, &new_server);
            else if (context == LOCATION_CONTEXT)
                set_directive(find_directive(directive, context), content, &new_location);
        }
    }
    throw ConfigParser::OpenContextBlockError();
};

// This function opens the _config_file, and then parses the file while looking for an opening server block (aka "server {")
// Then, calls context_parser() to parse the server block
void    ConfigParser::call()
{
    std::ifstream   file;
    std::string     line;
    std::string     directive;
    size_t          separator;

    try{
        file.open(this->_config_file);
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
    catch (std::runtime_error &e) {
		    ERROR(e.what());
    }
    file.close();
};

// _____________________________________________________________________________
// OLD call()
// void	ConfigParser::call( void )
// {
// 	// TODO: validate file exists or throw error
// 	// TODO: validate file contents or throw error
// 	// TODO: count server blocks in file
//     std::ifstream file;
//     file.open(_config_file.c_str(), std::ios::in);
// 	if (!file.is_open())
// 		throw(ConfigParser::InvalidConfigurationFileException());
// 	std::string line;
//     while (std::getline(file, line)) {
// 		line = strtrim(line);
// 		if (!line.length() || line[0] == '#')
// 			continue;
// 	}
// 	_configs_amount = 1; // Temporary
// 	_configs = Configs(_configs_amount);

// 	// Temp
// 	_configs[0].port = 8080;
// 	_configs[0].max_clients = 10;
// 	_configs[0].client_max_body_size = 10000;
// 	_configs[0].name = "Hello";
// }

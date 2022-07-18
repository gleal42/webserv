/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stuff_test.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmeira <fmeira@student.42lisboa.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/30 22:26:01 by msousa            #+#    #+#             */
/*   Updated: 2022/07/18 03:20:38 by fmeira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fstream>
#include <cstring>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <vector>
#include <algorithm>
#include <sys/stat.h>

# define SEPARATORS			            " \t\v\n\r\f"
# define CONTEXT_DIRECTIVES		        8
# define SERVER_CONTEXT		            1
# define LOCATION_CONTEXT	            2

typedef struct s_locations{
    std::vector<std::string>	_methods;
    std::string					_root_path;
    std::string					_target;
    std::vector<std::string>	_indexes;
    int							_max_body_size;
    bool						_autoindex;
    std::string					_cgi_bin;
} locations;

typedef std::vector<locations> Locations;

typedef struct s_config
{
	std::string					_server_name;
    std::string					_ip;
    int							_port;
	std::string					_root;
	std::string					_cgi_bin;
	std::string					_error_page;
	std::vector<std::string>	_allowed_methods;
	bool						_auto_index;
	std::vector<std::string>	_indexes;
	Locations   				_locations;
	int							_client_max_body_size;
	int							_max_clients;
	int							_request_timeout;
}   ServerConfig;

typedef std::vector<ServerConfig> Configs;


class ConfigParser {

    public:
        struct ConfigurationFileError : public std::runtime_error {
            ConfigurationFileError(void);
        };
        struct ConfigurationDirectiveError : public std::runtime_error {
            ConfigurationDirectiveError(const std::string err);
        };
        struct ConfigurationSyntaxError : public std::runtime_error {
            ConfigurationSyntaxError(const std::string err);
        };
        struct LocationPathError : public std::runtime_error {
            LocationPathError(const std::string err);
        };
        struct NestedServerContextError : public std::runtime_error {
            NestedServerContextError(void);
        };
        struct EmptyContextBlockError : public std::runtime_error {
            EmptyContextBlockError(void);
        };

        ConfigParser(char *str) : _config_file(str){};
        ~ConfigParser(void){};
        ConfigParser &	operator = ( ConfigParser const & rhs );

        // Getters
        ServerConfig	config(int const index) const;
        int				configs_amount(void) const;
        void            server_context_parser(std::ifstream* file_ref);
        void            ConfigParser::location_context_parser(std::string &path, ServerConfig *new_server, std::ifstream* file_ref);
        void            call(void);
        void            set_directive(std::string &directive, std::string &content, bool context, ServerConfig *new_server);
        bool            server_is_empty(ServerConfig test_server);
        void            set_location(std::string location, std::ifstream* file);

        // Config vector
        Configs			server_configs;

    private:

        ConfigParser( void );
        ConfigParser( ConfigParser const & src );
        char*			_config_file;
        int				_configs_amount;
};

/*---------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------*/
const std::string valid_server_directives[CONTEXT_DIRECTIVES] =
    {"root", "listen", "server_name", "error_page", "client_max_body_size",
    "location", "index", "autoindex"};

const std::string valid_location_directives[CONTEXT_DIRECTIVES] =
        {"root", "index", "allowed_methods", "autoindex",
        "error_page", "client_max_body_size", "cgi", "cgi-bin"};

/* Exceptions */
ConfigParser::ConfigurationFileError::ConfigurationFileError(void)
    : std::runtime_error("Error while opening configuration file") { /* No-op */ }

ConfigParser::ConfigurationDirectiveError::ConfigurationDirectiveError(const std::string err)
    : std::runtime_error("Error: " + err + "directive is invalid") { /* No-op */ }

ConfigParser::ConfigurationSyntaxError::ConfigurationSyntaxError(const std::string err)
    : std::runtime_error("Error. The following line contains invalid syntax:" + err) { /* No-op */ }

ConfigParser::LocationPathError::LocationPathError(const std::string err)
    : std::runtime_error("Error. The following path isn't accessible: " + err) { /* No-op */ }

ConfigParser::NestedServerContextError::NestedServerContextError(void)
    : std::runtime_error("Error trying to nest a server context") { /* No-op */ }

ConfigParser::EmptyContextBlockError::EmptyContextBlockError(void)
    : std::runtime_error("Error trying to add an empty context block") { /* No-op */ }


/* Config-parsing utils */
static std::string & strtrim(std::string & str) {
    str.erase(str.find_last_not_of(SEPARATORS) + 1);
    str.erase(0, str.find_first_not_of(SEPARATORS));
    return (str);
};

static bool check_directive_syntax(std::string directive){
    std::size_t found = directive.find_first_of(SEPARATORS);

    if (found == std::string::npos)
        return (false);

    std::string checking = directive.substr(found + 1);
    if (checking.length() < 3 && checking[checking.length() - 1] != ';')
        return (false);
}

static bool is_valid_directive(std::string directive, int type){
    int                 i = 0;
    const std::string   *valid_directives;

    if (type == LOCATION_CONTEXT)
        valid_directives = valid_location_directives;
    else if (type == SERVER_CONTEXT)
        valid_directives = valid_server_directives;
    while (i < CONTEXT_DIRECTIVES)
        if (directive == valid_directives[i++] && check_directive_syntax(directive))
            return (true);
    throw ConfigParser::ConfigurationDirectiveError(directive);
    return (false);
}

static bool isDirectory(const std::string & path) {
  struct stat s;

  if (lstat(path.c_str(), &s) == 0)
    if (S_ISDIR(s.st_mode))
      return (true);
  return (false);
}


/* ConfigParser methods */

bool    ConfigParser::server_is_empty(ServerConfig target){
    return (!target._allowed_methods.empty() || target._auto_index
        || !target._cgi_bin.empty() || target._client_max_body_size
        || !target._error_page.empty() || !target._indexes.empty()
        || !target._ip.empty() || !target._locations.empty()
        || target._max_clients || target._port || target._request_timeout
        || !target._root.empty() || !target._server_name.empty());
}

void    ConfigParser::location_context_parser(std::string &path, ServerConfig *new_server, std::ifstream* file_ref){

};
void    ConfigParser::set_directive(std::string &directive, std::string &content, bool context, ServerConfig *new_server){

};

void ConfigParser::server_context_parser(std::ifstream* file)
{
	std::string     line;
	std::string     directive;
    std::string     content;
    ServerConfig    new_server;
    static bool     is_global_context = true;

    while (std::getline(*file, line)) {
		line = strtrim(line);
		if (!line.length() || line[0] == '#')
			continue;
        if (line == "}"){
            if (is_global_context)
                throw ConfigurationSyntaxError(line);
            if (server_is_empty(new_server))
                throw ConfigParser::EmptyContextBlockError();
            this->server_configs.push_back(new_server);
            return ;
        }
        directive = line.substr(0, line.find_first_of(SEPARATORS));
        if (directive == "server"){
            if (!is_global_context)
                throw ConfigParser::NestedServerContextError();
            is_global_context = false;
            if (line[line.find_first_of(SEPARATORS) + 1] == '{'){
                server_context_parser(file);
                continue;
            }
            throw ConfigurationSyntaxError(line);
        }
        content = line.substr(line.find_first_of(SEPARATORS) + 1);
        if (directive == "location")
            location_context_parser(content, &new_server, file);
        else if (is_valid_directive(directive, SERVER_CONTEXT))
            set_directive(directive, content, is_global_context, &new_server);
        is_global_context = true;
    }
    if (this->server_configs.empty() && !server_is_empty(new_server))
        this->server_configs.push_back(new_server);
};

// void    tt(std::ifstream* file)
// {
//     std::string     line;
//     static int i = 0;

//     // while (*file) {
//         i++;
//         std::getline(*file, line);
//         std::cout << "tt()" << i << " -> " << line << "\n";
//         if (*file)
//             tt(file);
//     // }
// }

void ConfigParser::call(){
    std::ifstream   file;
    std::string     line;

    file.open(this->_config_file);
    if (!file.is_open()){
        std::cerr << "open failed\n";
        exit(1);
    }
    ConfigParser::server_context_parser(&file);
    file.close();
};

int main(int ac, char **av){
    if (ac == 2){
    ConfigParser config_parser(av[1]);
    config_parser.call();
    }

    return (0);
};

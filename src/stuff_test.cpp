/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stuff_test.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmeira <fmeira@student.42lisboa.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/30 22:26:01 by msousa            #+#    #+#             */
/*   Updated: 2022/07/16 03:29:12 by fmeira           ###   ########.fr       */
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

# define SEPARATORS			" \t\v\n\r\f"

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

        ConfigParser(char *str) : _config_file(str){};
        ~ConfigParser(void){};
        ConfigParser &	operator = ( ConfigParser const & rhs );

        // Getters
        ServerConfig	config(int const index) const;
        int				configs_amount(void) const;
        void            server_parser(std::ifstream* file_ref);
        void            location_parser(std::string &paths, std::ifstream* file_ref);
        void            call(void);
        void            set_directive(std::string &directive, std::string &content, std::ifstream* file);

        // Config vector
        Configs			_configs;

    private:

        ConfigParser( void );
        ConfigParser( ConfigParser const & src );
        char*			_config_file;
        int				_configs_amount;
};

/*---------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------*/

/* Exceptions */
ConfigParser::ConfigurationFileError::ConfigurationFileError(void)
    : std::runtime_error("Error while opening configuration file") { /* No-op */ }

ConfigParser::ConfigurationDirectiveError::ConfigurationDirectiveError(const std::string err)
    : std::runtime_error("Error: " + err + "directive is invalid") { /* No-op */ }

ConfigParser::ConfigurationSyntaxError::ConfigurationSyntaxError(const std::string err)
    : std::runtime_error("Error. The following line contains invalid syntax:" + err) { /* No-op */ }

ConfigParser::LocationPathError::LocationPathError(const std::string err)
    : std::runtime_error("Error. The following path isn't accessible: " + err) { /* No-op */ }

/* Config-parsing utils */
static std::string & strtrim(std::string & str) {
    str.erase(str.find_last_not_of(SEPARATORS) + 1);
    str.erase(0, str.find_first_not_of(SEPARATORS));
    return (str);
};

static bool is_valid_directive(std::string directive){
    std::vector<std::string>valid_server_directives =
        {"root", "listen", "server_name", "error_page", "client_max_body_size",
        "location", "index", "autoindex"};
    std::vector<std::string>::iterator found;

    found = find(valid_server_directives.begin(), valid_server_directives.end(), directive);
    if (found != valid_server_directives.end())
        return (true);
    return (false);
}

static bool is_valid_location_directive(std::string directive){
    std::vector<std::string>valid_location_directives =
        {"root", "index", "allowed_methods", "autoindex", "error_page",
        "client_max_body_size", "cgi", "cgi-bin"};
    std::vector<std::string>::iterator found;

    found = find(valid_location_directives.begin(), valid_location_directives.end(), directive);
    if (found != valid_location_directives.end())
        return (true);
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

void    ConfigParser::location_parser(std::string &paths, std::ifstream* file_ref){

};
void    ConfigParser::set_directive(std::string &directive, std::string &content, std::ifstream* file){

};

void ConfigParser::server_parser(std::ifstream* file)
{
	std::string     line;
	std::string     directive;
    std::string     content;
    ServerConfig    new_server;

    while (std::getline(*file, line)) {
		line = strtrim(line);
		if (!line.length() || line[0] == '#')
			continue;
        directive = line.substr(0, line.find_first_of(SEPARATORS));
        if (directive == "server"){
            if (line[line.find_first_of(SEPARATORS) + 1] == '{'){
                server_parser(file);
                continue;
            }
            throw ConfigurationSyntaxError(line);
        }
        if (is_valid_directive(directive)){
            content = line.substr(line.find_first_of(SEPARATORS) + 1);
            set_directive(directive, content, file);
        }
        if (line == "}"){
            this->_configs.push_back(new_server);
            return ;
        }
    }
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
    ConfigParser::server_parser(&file);
    file.close();
};

int main(int ac, char **av){
    ConfigParser config_parser(av[1]);
    config_parser.call();

    return (0);
};

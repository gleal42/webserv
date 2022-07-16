/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msousa <mlrcbsousa@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 08:37:16 by msousa            #+#    #+#             */
/*   Updated: 2022/06/23 09:55:21 by msousa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"

/* Exceptions */
const char * ConfigParser::InvalidConfigurationFileException::what() const throw() {return ("Invalid File, make sure you have permissions, that the file exists and the extension is .conf");}
const char * ConfigParser::InvalidDirectiveException::what() const throw() {return ("Directive is invalid");}
const char * ConfigParser::WrongSyntaxException::what() const throw() {return ("Wrong Directive Syntax");}

/* Constructors */
ConfigParser::ConfigParser( void ) : _config_file(""), _configs_amount(0) { /* no-op */ }
ConfigParser::ConfigParser( ConfigParser const & src ) { *this = src; }
ConfigParser::ConfigParser(std::string config_file) : _config_file(config_file),
	_configs_amount(0) { /* no-op */ }

// Getters
ServerConfig	ConfigParser::config( int const index ) const { return _configs[index]; }
int	ConfigParser::configs_amount( void ) const { return _configs_amount; }

/* Destructor */
ConfigParser::~ConfigParser( void ) { /* no-op */ }

/* Assignment operator */
ConfigParser &	ConfigParser::operator = ( ConfigParser const & rhs )
{
	if (this != &rhs) {
		//value = rhs.value;
	}
	return *this;
}

static std::string & strtrim(std::string & str) {
    str.erase(str.find_last_not_of(SEPARATORS) + 1);
    str.erase(0, str.find_first_not_of(SEPARATORS));
    return (str);
}

void	ConfigParser::call( void )
{
	// TODO: validate file exists or throw error
	// TODO: validate file contents or throw error
	// TODO: count server blocks in file
    std::ifstream file;
    file.open(_config_file.c_str(), std::ios::in);
	if (!file.is_open())
		throw(ConfigParser::InvalidConfigurationFileException());
	std::string line;
    while (std::getline(file, line)) {
		line = strtrim(line);
		if (!line.length() || line[0] == '#')
			continue;
	}
	_configs_amount = 1; // Temporary
	_configs = Configs(_configs_amount);

	// Temp
	_configs[0].port = 8080;
	_configs[0].max_clients = 10;
	_configs[0].client_max_body_size = 10000;
	_configs[0].name = "Hello";

}

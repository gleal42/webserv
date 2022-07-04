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

void	ConfigParser::call( void )
{
	// TODO: validate file exists or throw error
	// TODO: validate file contents or throw error
	// TODO: count server blocks in file
	_configs_amount = 1; // Temporary
	_configs = Configs(_configs_amount);

	// Temp
	_configs[0].port = 8080;
	_configs[0].max_clients = 10;
	_configs[0].input_buffer_size = 10000;
	_configs[0].name = "Hello";

}

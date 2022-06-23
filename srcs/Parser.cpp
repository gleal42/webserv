/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msousa <mlrcbsousa@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 08:37:16 by msousa            #+#    #+#             */
/*   Updated: 2022/06/23 09:55:21 by msousa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

/* Constructors */
Parser::Parser( void ) : _config_file(""), _configs_amount(0) { /* no-op */ }
Parser::Parser( Parser const & src ) { *this = src; }
Parser::Parser(std::string config_file) : _config_file(config_file),
	_configs_amount(0) { /* no-op */ }

// Getters
ServerConfig	Parser::config( int const index ) const { return _configs[index]; }
int	Parser::configs_amount( void ) const { return _configs_amount; }

/* Destructor */
Parser::~Parser( void ) { /* no-op */ }

/* Assignment operator */
Parser &	Parser::operator = ( Parser const & rhs )
{
	if (this != &rhs) {
		//value = rhs.value;
	}
	return *this;
}

void	Parser::call( void )
{
	// TODO: validate file exists or throw error
	// TODO: validate file contents or throw error
	// TODO: count server blocks in file
	_configs_amount = 1; // Temporary
	_configs = Configs(_configs_amount);
}

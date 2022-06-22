/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msousa <mlrcbsousa@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 08:37:35 by msousa            #+#    #+#             */
/*   Updated: 2022/06/22 09:00:42 by msousa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __PARSER_H__
# define __PARSER_H__

# include <iostream>

# include "ServerConfig.hpp"

// ************************************************************************** //
//                               Parser Class                             	  //
// ************************************************************************** //

/*
A class to parse the config_file and turn it into ServerConfig objects

Needs to be able to:

	- Return errors if not valid
	- Create as many ServerConfigs as `server` blocks in the config_file

*/

class Parser {

public:

	Parser(std::string config_file);
	Parser( Parser const & src );
	~Parser( void );
	Parser &	operator = ( Parser const & rhs );

	// Getters
	ServerConfig *	config( int const index ) const;
	int	const		configs_amount( void ) const;

	// bool			is_valid( void );

private:

	Parser( void );
	int				_configs_amount;
	std::string		_config_file;
	ServerConfig *	_configs[];

};

std::ostream &	operator << ( std::ostream & o, Parser const & i );

#endif /* __PARSER_H__ */

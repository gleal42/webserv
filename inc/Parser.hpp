/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msousa <mlrcbsousa@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 08:37:35 by msousa            #+#    #+#             */
/*   Updated: 2022/06/23 09:55:15 by msousa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __PARSER_H__
# define __PARSER_H__

# include <iostream>
# include <vector>

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

typedef std::vector<ServerConfig> Configs;

class Parser {

public:

	Parser(std::string config_file);
	~Parser( void );
	Parser &	operator = ( Parser const & rhs );

	// Getters
	ServerConfig	config( int const index ) const;
	int				configs_amount( void ) const;

	void			call( void );

private:

	Parser( void );
	Parser( Parser const & src );
	std::string		_config_file;
	int				_configs_amount;
	Configs			_configs;

};

#endif /* __PARSER_H__ */

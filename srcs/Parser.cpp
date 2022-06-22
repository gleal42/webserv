/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msousa <mlrcbsousa@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 08:37:16 by msousa            #+#    #+#             */
/*   Updated: 2022/06/22 08:37:24 by msousa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

/* Constructors */
Parser::Parser( void ) { /* no-op */ }
Parser::Parser( Parser const & src ) { *this = src; }

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

/* ostream override */
std::ostream &	operator << ( std::ostream & o, Parser const & i )
{
	(void)i;
	o << "Parser";
	return o;
}

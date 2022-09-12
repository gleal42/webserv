/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msousa <mlrcbsousa@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/12 19:09:38 by gleal             #+#    #+#             */
/*   Updated: 2022/09/11 17:46:56 by msousa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Connection.hpp"

Connection::Connection( Listener * listener )
try : Socket(*listener->accept()) { /* no-op */ }
catch(Socket::AcceptError & e) {
	ERROR(e.what());
}

Connection::Connection( const Connection & src ) : Socket(src)
{
	throw std::runtime_error("Connection copy constructor is private");
}

Connection::~Connection( void ) { /* no-op */ }

Connection & Connection::operator=( const Connection & src )
{
	(void)src;
	throw std::runtime_error("Connection assignment operator is private");
}

Connection::Connection( void )
{
	throw std::runtime_error("Default constructor is private");
}

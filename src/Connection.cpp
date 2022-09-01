/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/12 19:09:38 by gleal             #+#    #+#             */
/*   Updated: 2022/08/31 21:35:32 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Connection.hpp"

Connection::Connection( Listener * listener )
try : Socket(*listener->accept()) { /* no-op */ }
catch(Socket::AcceptError& e) {
	LOG(e.what());
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

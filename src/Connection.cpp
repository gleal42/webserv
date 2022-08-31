/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msousa <mlrcbsousa@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/12 19:09:38 by gleal             #+#    #+#             */
/*   Updated: 2022/08/31 15:27:41 by msousa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Connection.hpp"

Connection::Connection( Listener * listener )
try : Socket(*listener->accept()) { /* no-op */ }
catch(Socket::AcceptError& e) {
	LOG(e.what());
}
// {
// 	try {
// 		_socket = listener->accept();
// 	}
// 	catch(Socket::AcceptError& e) {
// 		LOG(e.what());
// 	}
// 	// _socket = listener->accept();
// 	// if (!_socket)
// 	// 	throw std::runtime_error("Couldn't accept"); // add error here
// }

Connection::Connection( const Connection & src ) : Socket(src)
{
	throw std::runtime_error("Connection copy constructor is private");
	// request = src.request;
	// response = src.response;
	// _socket = new Socket(*src._socket);
}

Connection::~Connection( void ) { /* no-op */ }

Connection & Connection::operator=( const Connection & src )
{
	(void)src;
	throw std::runtime_error("Connection assignment operator is private");
	// request = src.request;
	// response = src.response;
	// _socket = new Socket(*src._socket);
}

Connection::Connection( void )
{
	throw std::runtime_error("Default constructor is private");
}

// int Connection::fd()
// {
//     return (_socket->fd());
// }

// Socket *	Connection::socket()
// {
//     return (_socket);
// }

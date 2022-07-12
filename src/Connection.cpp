/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/12 19:09:38 by gleal             #+#    #+#             */
/*   Updated: 2022/07/12 20:57:30 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Connection.hpp"
# include <stdexcept>

Connection::Connection( Socket * listener_socket )
{
	_socket = listener_socket->accept();
	if (!_socket)
		throw std::runtime_error("Couldn't accept"); // add error here
}

Connection::Connection( const Connection & src )
{
	throw std::runtime_error("Connection copy constructor is supposed to be private");
	request = src.request;
	response = src.response;
	_socket = new Socket(*src._socket);
}

Connection::~Connection()
{

}

Connection & Connection::operator=( const Connection & src )
{
	throw std::runtime_error("Connection = oprator is supposed to be private");
	request = src.request;
	response = src.response;
	_socket = new Socket(*src._socket);
}

Connection::Connection( void )
{
	throw std::runtime_error("Default constructor should be private");
}

int Connection::fd()
{
    return (_socket->fd());
}

Socket *	Connection::socket()
{
    return (_socket);
}

void	Connection::shutdown( void )
{
	_socket->close();
}
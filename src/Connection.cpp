/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msousa <mlrcbsousa@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/12 19:09:38 by gleal             #+#    #+#             */
/*   Updated: 2022/08/26 17:21:51 by msousa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Connection.hpp"

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

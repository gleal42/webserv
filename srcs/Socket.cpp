/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 18:31:55 by msousa            #+#    #+#             */
/*   Updated: 2022/07/01 16:15:57 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

/* Exceptions */
Socket::BindError::BindError( void ) : std::runtime_error("") { /* No-op */ }

Socket::CreateError::CreateError( void )
	: std::runtime_error("Failed to create socket.") { /* No-op */ }

Socket::ReusableAddressError::ReusableAddressError( void )
	: std::runtime_error("Failed to make socket address reusable.") { /* No-op */ }

Socket::ReusablePortError::ReusablePortError( void )
	: std::runtime_error("Failed to make socket port reusable.") { /* No-op */ }

Socket::BindError::BindError( int port )
	: std::runtime_error("Failed to bind to port " + std::to_string(port) + ".")
	{ /* No-op */ }

Socket::ListenError::ListenError( void )
	: std::runtime_error("Failed to listen on socket.") { /* No-op */ }

Socket::AcceptError::AcceptError( void )
	: std::runtime_error("Failed to Accept new connection.") { /* No-op */ }

/* Constructors */
Socket::Socket( void ) : _fd(FD_UNSET), _port(PORT_UNSET) { /* No-op */ }

// TODO: will we also pass `domain`?
Socket::Socket( int port ) : _port(PORT_UNSET)
{
	create();
	setsockopt(SO_REUSEPORT);
	setsockopt(SO_REUSEADDR);
	bind(port);
}

Socket::Socket( Socket const & src ) { *this = src; }

/* Destructor */
Socket::~Socket( void )
{
	// close();
}

/* Assignment operator */
Socket &	Socket::operator = ( Socket const & rhs )
{
	if (this != &rhs) {
		_fd = rhs._fd;;
		_address = rhs._address;;
		_port = rhs._port;;
		_buffer = rhs._buffer;;
	}
	return *this;
}

// Getters
int	Socket::fd( void ) { return _fd; }
int	Socket::port( void ) { return _port; }

// Setters
void	Socket::set_fd( int fd ) { _fd = fd; }

// C `socket` function wrapper
void	Socket::create( void )
{
	_fd = socket(AF_INET, SOCK_STREAM, 0);

  	if (_fd == FD_UNSET) {
		throw Socket::CreateError();
	}
	fcntl(_fd, F_SETFL, O_NONBLOCK); // Setting socket to NONBLOCKING
}

// C 'setsockopt' function wrapper for setting Socket port as reusable
void	Socket::setsockopt( int option )
{
    const int val = 1;
	if (::setsockopt(_fd, SOL_SOCKET, option, &val, sizeof(int)) < 0)
	{
		if (option == SO_REUSEPORT)
			throw Socket::ReusablePortError();
		else if (option == SO_REUSEADDR)
			throw Socket::ReusableAddressError();
	}
}

// C `bind` function wrapper
void	Socket::bind( int port )
{
	memset(&_address, 0, sizeof(struct sockaddr_in));
	_address.sin_family = AF_INET;
	_address.sin_addr.s_addr = htonl(INADDR_ANY);
	_address.sin_port = htons(port);
	if (::bind(_fd, (sockaddr *)&_address, sizeof(_address)) < 0) {
		throw Socket::BindError(port);
	}
	_port = port;	// only set port if did't fail `bind` call
}

// C `close` function wrapper
void	Socket::close( void ) { if(_fd != FD_UNSET ) ::close(_fd); }

// C `listen` function wrapper
void	Socket::listen( int max_connections ) { // Coming from server config or should be const?
	if (_port == PORT_UNSET || ::listen(_fd, max_connections) < 0) {
		throw Socket::ListenError();
	}
}

// `send` function wrapper
void	Socket::send( const std::string & response ) {
	::send(_fd, response.c_str(), response.size(), 0);
}

// `recv` function wrapper
int	Socket::receive( int buffer_size ) {
	// https://stackoverflow.com/questions/51318393/recv-function-for-tcp-socket-in-c
	_buffer = std::vector<char>(buffer_size, '\0');

	return recv(_fd, _buffer.data(), _buffer.size(), 0);
}

std::string	Socket::to_s( void ) { return std::string(_buffer.data()); }

// C `accept` function wrapper
Socket *	Socket::accept( void ) {
	Socket *	s = new Socket();

	// TODO: Need to check that these vars are actually set on new socket
	socklen_t	length = sizeof(s->_address);
	sockaddr *	address = (sockaddr *)&s->_address;

	s->set_fd(::accept(_fd, address, &length));
	if ((s->fd() == FD_UNSET)) {
		delete s;
		return NULL; // or something else later
        throw Socket::AcceptError();
	}
	fcntl(s->fd(), F_SETFL, O_NONBLOCK);
	return s;
}

/* ostream override */
std::ostream &	operator << ( std::ostream & o, Socket const & i )
{
	(void)i;
	o << "Socket";
	return o;
}

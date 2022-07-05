/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msousa <mlrcbsousa@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 18:31:55 by msousa            #+#    #+#             */
/*   Updated: 2022/07/05 01:24:18 by msousa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

/* Exceptions */
Socket::BindError::BindError( void ) : std::runtime_error("") { /* No-op */ }

Socket::CreateError::CreateError( void )
	: std::runtime_error("Failed to create socket.") { /* No-op */ }

Socket::BindError::BindError( int port )
	: std::runtime_error("Failed to bind to port " + to_string(port) + ".")
	{ /* No-op */ }

Socket::ListenError::ListenError( void )
	: std::runtime_error("Failed to listen on socket.") { /* No-op */ }

/* Constructors */
Socket::Socket( void ) : _port(PORT_UNSET), _fd(FD_UNSET), _bytes(0) { /* No-op */ }

// TODO: will we also pass `domain`?
Socket::Socket( int port ) : _port(PORT_UNSET), _bytes(0)
{
	create();
	bind(port);
}

Socket::Socket( Socket const & src ) { *this = src; }

/* Destructor */
Socket::~Socket( void ) { /* No-op */ }

/* Assignment operator */
Socket &	Socket::operator = ( Socket const & rhs )
{
	if (this != &rhs) {
		_fd = rhs._fd;
		_port = rhs._port;
		_address = rhs._address;
		_buffer = rhs._buffer;
		_bytes = rhs._bytes;
	}
	return *this;
}

// Getters
int	Socket::fd( void ) const { return _fd; }
int	Socket::port( void ) const { return _port; }
int	Socket::bytes( void ) const { return _bytes; }

// Setters
void	Socket::set_fd( int fd ) { _fd = fd; }

// C `socket` function wrapper
void	Socket::create( void )
{
	_fd = socket(AF_INET, SOCK_STREAM, 0);
  	if (_fd == FD_UNSET) {
		throw Socket::CreateError();
	}
	// fcntl(_fd, F_SETFL, O_NONBLOCK);
}

// C `bind` function wrapper
void	Socket::bind( int port )
{
	memset(&_address, 0, sizeof(SocketAddress));
	_address.sin_family = AF_INET;
	_address.sin_addr.s_addr = htonl(INADDR_ANY);
	_address.sin_port = htons(port);
	if (::bind(_fd, (sockaddr *)&_address, sizeof(_address))) {
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
	// You should generally check that the number of bytes sent was as expected,
	// and you should attempt to send the rest if it's not.
}

// `recv` function wrapper
void	Socket::receive( void ) {
	// The following code uses recv() to write new data into the client's buffer
	// while being careful to not overflow that buffer:

	_bytes += recv(_fd, _buffer.data() + _bytes, _buffer.size() - _bytes, 0);

	// TODO: do something if `recv` fails by returning 0 or -1.
	// If the connection is terminated by the client, recv() returns 0 or -1,
	// depending on the circumstance.
}
// It is a common mistake to try printing data that's received from recv() directly
// as a C string. There is no guarantee that the data received from recv() is null
// terminated! If you try to print it with printf(request) or printf("%s", request),
// you will likely receive a segmentation fault error (or at best it will print some
// garbage).

std::string	Socket::to_s( void ) const { return std::string(_buffer.data()); }

// C `accept` function wrapper
Socket *	Socket::accept( int buffer_size ) {
	Socket *	s = new Socket();

	// TODO: Need to check that these vars are actually set on new socket
	socklen_t	length = sizeof(s->_address);
	sockaddr *	address = (sockaddr *)&s->_address;

	s->set_fd(::accept(_fd, address, &length));
	if ((s->fd() == FD_UNSET)) {
		LOG(strerror(errno)); // Temporary to debug
		delete s;
		return NULL; // or something else later
	}

	// https://stackoverflow.com/questions/51318393/recv-function-for-tcp-socket-in-c
	s->_buffer = std::vector<char>(buffer_size, '\0');

	// fcntl(s->fd(), F_SETFL, O_NONBLOCK);
	return s;
}

/* ostream override */
std::ostream &	operator << ( std::ostream & o, Socket const & i )
{
	o << i.to_s();
	return o;
}

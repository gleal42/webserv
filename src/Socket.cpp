/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msousa <mlrcbsousa@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 18:31:55 by msousa            #+#    #+#             */
/*   Updated: 2022/07/15 01:40:54 by msousa           ###   ########.fr       */
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
	: std::runtime_error("Failed to bind to port " + to_string(port) + ".")
	{ /* No-op */ }

Socket::ListenError::ListenError( void )
	: std::runtime_error("Failed to listen on socket.") { /* No-op */ }

Socket::AcceptError::AcceptError( void )
	: std::runtime_error("Failed to Accept new connection.") { /* No-op */ }

/* Constructors */
Socket::Socket( void ) : _port(PORT_UNSET), _fd(FD_UNSET), _bytes(0){ /* No-op */ }

// Changed to config parameter so that we could copy parent request to connections socket
// TODO: will we also pass `domain`?
Socket::Socket( ServerConfig config ) : _port(PORT_UNSET), _fd(FD_UNSET), _bytes(0)
{
	create();
	setsockopt(SO_REUSEPORT);
	setsockopt(SO_REUSEADDR);
	bind(config.port);
}

Socket::Socket( Socket const & src ) { *this = src; }

/* Destructor */
Socket::~Socket( void ) { this->close(); }

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
int	Socket::send( const std::string & response ) const {
	return (::send(_fd, response.c_str(), response.size(), 0));
}

// `recv` function wrapper
void	Socket::receive( int buffer_size ) {
	// https://stackoverflow.com/questions/51318393/recv-function-for-tcp-socket-in-c
	_buffer = std::vector<char>(buffer_size + 1, '\0'); // One more for null terminated string

	_bytes = recv(_fd, _buffer.data(), buffer_size, 0);
	std::cout << "We just received " << _bytes << " bytes." << std::endl;
	if (buffer_size != _bytes)
		throw std::runtime_error("UH OHHHHHHHHHHHH");
	// std::cout << "The data received was :" << std::endl;
	// std::cout << _buffer.data() << std::endl;
}
// It is a common mistake to try printing data that's received from recv() directly
// as a C string. There is no guarantee that the data received from recv() is null
// terminated! If you try to print it with printf(request) or printf("%s", request),
// you will likely receive a segmentation fault error (or at best it will print some
// garbage).

std::string	Socket::to_s( void ) const { return std::string(_buffer.data()); }

// C `accept` function wrapper
Socket *	Socket::accept( void ) {
	Socket *	s = new Socket();

	// TODO: Need to check that these vars are actually set on new socket
	socklen_t	length = sizeof(s->_address);
	sockaddr *	address = (sockaddr *)&s->_address;

	s->set_fd(::accept(_fd, address, &length));
	if ((s->fd() == FD_UNSET)) {
		delete s;
        throw Socket::AcceptError();
	}
	fcntl(s->fd(), F_SETFL, O_NONBLOCK);

	return s;
}

/* ostream override */
std::ostream &	operator << ( std::ostream & o, Socket const & i )
{
	o << i.to_s();
	return o;
}

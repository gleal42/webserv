#ifndef WEBSERVER_HPP
# define WEBSERVER_HPP

#include <iostream>
#include <string>
#include <sys/event.h>
#include <sys/socket.h>
#include <sys/time.h>

#include "macros.hpp"
#include "Server.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "ServerConfig.hpp"
#include "Socket.hpp"
#include "Kqueue.hpp"

struct KqueueError : public std::runtime_error {
	KqueueError( void );
};

int webserver(std::string config);

#endif

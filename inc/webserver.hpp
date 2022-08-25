/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msousa <mlrcbsousa@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 16:09:11 by msousa            #+#    #+#             */
/*   Updated: 2022/08/25 23:12:27 by msousa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERVER_HPP
# define WEBSERVER_HPP

# include <iostream>
# include <string>
# include <sys/socket.h>
# include <sys/time.h>
# include <map>
# include <cstdlib> // For EXIT_FAILURE, etc, macros
# include <sstream>

# if defined(DARWIN)
#  include <sys/event.h>
#  define QUEUE() kqueue()
#  define EVENT struct kevent
#  define TIMEOUT NULL
# endif
# if defined(LINUX)
#  include <sys/epoll.h>
// -->	int epoll_create(int size);
//	The size argument is an indication to the kernel about the number of file descriptors
//	a process wants to monitor, which helps the kernel to decide the size of the epoll
//	instance. Since Linux 2.6.8, this argument is ignored because the epoll data structure
//	dynamically resizes as file descriptors are added or removed from it.
#  define QUEUE() epoll_create(1)
#  define EVENT struct epoll_event
#  define TIMEOUT -1
# endif

# include "macros.hpp"

typedef std::map<int, std::string> HTTPStatuses;

enum HTTPStatusGroup {
	STATUS_INVALID,
	STATUS_INFO,
	STATUS_SUCCESS,
	STATUS_REDIRECT,
	STATUS_CLIENT_ERROR,
	STATUS_SERVER_ERROR,
};

// Didn't work to include all header files here, if someone wants to give it
// a shot whilst still being able to `make`

// Functions
int 				webserver(std::string config);
std::string const	http_phrase( int code );
HTTPStatusGroup		http_group( int code );
std::string 		get_extension( const std::string &filename );
std::string			to_string( int number );
int					str_to_hexa(std::string hexa_nbr);

#endif

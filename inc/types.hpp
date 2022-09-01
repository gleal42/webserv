/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmeira <fmeira@student.42lisboa.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/26 16:49:12 by msousa            #+#    #+#             */
/*   Updated: 2022/09/01 13:03:36 by fmeira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __TYPES_HPP__
# define __TYPES_HPP__

/* All Domain Type Knowledge */

# include <vector>
# include <string>
# include <map>
# include <set>
# include <netinet/in.h>
# include <netinet/in.h>

# if defined(DARWIN)
#  include <sys/event.h>
#  define EVENT struct kevent
# endif
# if defined(LINUX)
#  include <sys/epoll.h>
#  define EVENT struct epoll_event
# endif

class	BaseStatus;
class	ConfigParser;
class	Connection;
class	Event;
class	FileHandler;
class	Handler;
class	Listener;
class	Request;
class	Response;
class	Server;
class 	ServerConfig;
class	Socket;
class	URI;

struct	ServerConfig;
// TODO: add error structs?

enum RequestMethod {
	GET,
	POST,
	DELETE,
};

/* Building Blocks */
typedef std::string					String;
typedef std::map<String, String>	StringMap;

typedef std::vector<ServerConfig> 		Configs;
typedef std::map<int, Connection *>		Connections;
typedef Connections::iterator			Connections_it;
typedef std::set<String>				Extensions;
typedef StringMap						Parameters;
typedef sockaddr_in						SocketAddress;
typedef std::map<int, Listener *>		Listeners;
typedef Listeners::iterator				Listener_it;
typedef StringMap						ResponseHeaders;
typedef StringMap						RequestHeaders;
typedef std::map<String, RequestMethod>	RequestMethods;
typedef	ResponseHeaders::iterator		ResponseHeaders_it;
typedef std::map<int, String>			HTTPStatuses;
typedef StringMap						MimeTypes;

#endif

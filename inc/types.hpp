/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/26 16:49:12 by msousa            #+#    #+#             */
/*   Updated: 2022/09/11 17:06:57 by gleal            ###   ########.fr       */
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
// class	BaseConfig;
class 	LocationConfig;
class 	ServerConfig;
class	Socket;
class	URI;

enum RequestMethod {
	GET,
	POST,
	DELETE,
};

// ServerConfig data structs
enum directives {
    DIRECTIVE_ROOT,
    DIRECTIVE_AUTOINDEX,
    DIRECTIVE_ERRORPAGE,
    DIRECTIVE_MAXBODYSIZE,
    DIRECTIVE_INDEX,
    DIRECTIVE_REDIRECT,
    DIRECTIVE_CGI = 6,
    DIRECTIVE_LIMITEXCEPT = 7,
    DIRECTIVE_LISTEN = 6,
    DIRECTIVE_SERVERNAME = 7,
};

enum AutoBool {
    AUTOINDEX_OFF,
    AUTOINDEX_ON,
    AUTOINDEX_UNSET
};

struct Listen {
    std::string	ip;
    int        	port;
    bool       	is_set;
};

struct CGI {
    std::string	extension;
    std::string	interpreter;
    bool		is_configured(const std::string &extension) const;
    bool		empty() const;
};

struct Redirect {
    unsigned short	code;
    std::string   	new_path;
};

/* Building Blocks */
typedef std::string												String;
typedef std::map<String, String>								StringMap;
typedef std::vector<String>										StringVector;

typedef StringMap												Parameters;
typedef StringMap												ResponseHeaders;
typedef StringMap												RequestHeaders;
typedef StringMap												MimeTypes;
typedef std::vector<ServerConfig> 								Configs;
typedef std::map<int, Connection *>								Connections;
typedef std::set<String>										Extensions;
typedef sockaddr_in 											SocketAddress;
typedef std::map<int, Listener *>								Listeners;
typedef std::map<String, RequestMethod>							RequestMethods;
typedef std::map<int, String>									HTTPStatuses;
typedef std::map<std::string, LocationConfig>					Locations;
typedef StringVector											Indexes;
typedef std::map< std::string, std::vector<unsigned short> >	ErrorPage;

// Iterators
typedef StringVector::iterator									StringVector_it;
typedef StringVector::const_iterator							StringVector_cit;
typedef	ResponseHeaders::iterator								ResponseHeaders_it;
typedef Listeners::iterator										Listener_it;
typedef Connections::iterator									Connections_it;
typedef ErrorPage::iterator										ErrorPage_it;
typedef ErrorPage::const_iterator								ErrorPage_cit;
typedef Locations::const_iterator								Location_cit;
typedef Indexes::const_iterator									Indexes_cit;

#endif

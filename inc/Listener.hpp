/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Listener.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msousa <mlrcbsousa@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/11 22:19:09 by gleal             #+#    #+#             */
/*   Updated: 2022/08/26 17:56:22 by msousa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __LISTENER_H__
# define __LISTENER_H__

# include <iostream>
# include <sys/socket.h>
# include <fcntl.h>

# include "Socket.hpp"
# include "ServerConfig.hpp"
# include "types.hpp"

// https://en.wikipedia.org/wiki/List_of_HTTP_header_fields

class Listener {

public:

	Listener( Listener const & src );
	Listener( ServerConfig const & config );
	~Listener( void );
	Listener &		operator = ( Listener const & rhs );

	void			stop( void );
	int				fd( void );
	Socket *		socket( void );

	ServerConfig	_config;

private:

	Listener( void );
	Socket *		_socket;

};

#endif /* __LISTENER_H__ */

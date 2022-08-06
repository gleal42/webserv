/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msousa <mlrcbsousa@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/12 15:59:39 by gleal             #+#    #+#             */
/*   Updated: 2022/08/06 19:24:37 by msousa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __CONNECTION_H__
# define __CONNECTION_H__

#include "Socket.hpp"
#include "Request.hpp"
#include "Response.hpp"

class Connection
{
public:
	Connection( Socket * listener_socket );
	~Connection( );
	int	fd( void );
	Socket *socket( void );
	Request request;
	Response response;
private:
	Connection( void );
	Connection( const Connection & src );
	Connection &operator=( const Connection & src );
	Socket *		_socket;
};

typedef std::map< int, Connection * > Connections;
typedef Connections::iterator Connections_it;

#endif

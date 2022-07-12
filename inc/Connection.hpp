/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/12 15:59:39 by gleal             #+#    #+#             */
/*   Updated: 2022/07/12 22:20:54 by gleal            ###   ########.fr       */
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
	~Connection();
	int	fd();
	Socket *socket();

	Request request;
	Response response;
private:
	Connection( void );
	Connection( const Connection & src );
	Connection &operator=( const Connection & src );
	Socket *		_socket;
};

typedef std::map< int, Connection * > Connections;
typedef Connections::iterator ConnectionsIter;

#endif

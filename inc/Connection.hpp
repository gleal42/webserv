/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msousa <mlrcbsousa@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/12 15:59:39 by gleal             #+#    #+#             */
/*   Updated: 2022/08/26 17:43:52 by msousa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __CONNECTION_H__
# define __CONNECTION_H__

# include <stdexcept>

# include "Socket.hpp"
# include "Request.hpp"
# include "Response.hpp"
# include "types.hpp"

class Connection {

public:

	Connection( Socket * listener_socket );
	~Connection( void );

	int			fd( void );
	Socket 		*socket( void );

	Request 	request;
	Response	response;

private:

	Connection( void );
	Connection( const Connection & src );
	Connection &	operator=( const Connection & src );
	Socket *		_socket;

};

#endif

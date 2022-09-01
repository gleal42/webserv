/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Listener.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/11 22:19:09 by gleal             #+#    #+#             */
/*   Updated: 2022/08/31 21:42:07 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __LISTENER_H__
# define __LISTENER_H__

# include "Socket.hpp"
# include "types.hpp"

// https://en.wikipedia.org/wiki/List_of_HTTP_header_fields

class Listener : public Socket {

public:

	Listener( ServerConfig const & config );
	Listener( Listener const & src );
	~Listener( void );
	Listener &		operator = ( Listener const & rhs );

private:

	Listener( void );

};

#endif /* __LISTENER_H__ */

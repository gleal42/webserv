/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Listener.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msousa <mlrcbsousa@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/11 22:16:37 by gleal             #+#    #+#             */
/*   Updated: 2022/08/30 22:25:33 by msousa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Listener.hpp"

/* Constructors */

Listener::Listener( void ) : Socket() { /* no-op */ }
Listener::Listener( Listener const & src ) : Socket() { *this = src; }

Listener::Listener( ServerConfig const & config ) : Socket(config)
{
	// there should always be a socket at this point,
	// the catches above should stop flow
	// maybe move them to main Listener initialization in `webserver()`
	try {
		listen(config.max_clients);
	}
	catch(Socket::ListenError& e) {
		LOG(e.what());
	}
}

/* Destructor */
Listener::~Listener( void ) { /* no-op */ }

/* Assignment operator */
Listener &	Listener::operator = ( Listener const & rhs )
{
	if (this != &rhs) {
		Socket::operator = (rhs);
	}
	return *this;
}

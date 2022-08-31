/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Listener.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmeira <fmeira@student.42lisboa.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/11 22:16:37 by gleal             #+#    #+#             */
/*   Updated: 2022/08/09 20:52:23 by fmeira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Listener.hpp"

/* Constructors */
Listener::Listener( void ) : Socket() { /* no-op */ }
Listener::Listener( Listener const & src ) : Socket() { *this = src; }

// so no need for config argument
Listener::Listener( ServerConfig const & config ) : Socket(config)
{
	try {
		listen(SOMAXCONN);
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

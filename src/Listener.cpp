/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Listener.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msousa <mlrcbsousa@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/11 22:16:37 by gleal             #+#    #+#             */
/*   Updated: 2022/09/17 01:42:29 by msousa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Listener.hpp"
#define MAX_CLIENTS 10 //not defined in config_file

/* Constructors */
Listener::Listener( void ) : Socket() { /* no-op */ }
Listener::Listener( Listener const & src ) : Socket() { *this = src; }

Listener::Listener( ServerConfig const & config ) : Socket(config), _config(config) {
	try {
		listen(MAX_CLIENTS);
	}
	catch(Socket::ListenError & e) {
		ERROR(e.what());
		throw ;
	}
}

/* Destructor */
Listener::~Listener( void ) { /* no-op */ }

/* Assignment operator */
Listener &	Listener::operator = ( Listener const & rhs ) {
	if (this != &rhs) {
		Socket::operator = (rhs);
	}
	return *this;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Event.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msousa <mlrcbsousa@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/26 15:52:30 by msousa            #+#    #+#             */
/*   Updated: 2022/08/26 16:34:02 by msousa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Event.hpp"

/* Constructors */
Event::Event( void ) { /* no-op */ }
Event::Event( Event const & src ) { *this = src; }
Event::Event( EVENT const & event_os ): _event_os(event_os) { /* no-op */ }

/* Destructor */
Event::~Event( void ) { /* no-op */ }

/* Assignment operator */
Event &	Event::operator = ( Event const & rhs )
{
	if (this != &rhs) {
		_event_os = rhs._event_os;
	}
	return *this;
}

/* Getters */
int	Event::fd( void ) const
{
#if defined(DARWIN)
	return _event_os.ident;
#endif

#if defined(LINUX)
	return _event_os.data.fd;
#endif
}

// int	Event::filter( void ) const
// {}

// int	Event::flags( void ) const
// {}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Event.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmeira <fmeira@student.42lisboa.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/26 15:52:30 by msousa            #+#    #+#             */
/*   Updated: 2022/09/17 01:20:44 by fmeira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Event.hpp"

EVENT	empty_event = {};

/* Constructors */
Event::Event( EVENT const & event_os ): _event_os(event_os) { /* no-op */ }

/* Private Constructors */
Event::Event( void ): _event_os(empty_event) { throw std::runtime_error("Do not use!"); }
Event::Event( Event const & src ): _event_os(empty_event) { (void)src; throw std::runtime_error("Do not use!"); }
Event &	Event::operator = ( Event const & rhs ) { (void)rhs; throw std::runtime_error("Do not use!"); return *this; }

/* Destructor */
Event::~Event( void ) { /* no-op */ }

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

bool	Event::is_read( void )
{
#if defined(DARWIN)
	return _event_os.filter == EVFILT_READ;
#endif
#if defined(LINUX)
	return _event_os.events & (EPOLLIN | EPOLLERR);
#endif
}

bool	Event::is_write( void )
{
#if defined(DARWIN)
	return _event_os.filter == EVFILT_WRITE;
#endif
#if defined(LINUX)
	return _event_os.events & (EPOLLOUT | EPOLLERR);
#endif
}

bool	Event::is_close( void )
{
#if defined(DARWIN)
	return _event_os.flags & EV_EOF;
#endif
#if defined(LINUX)
	return (_event_os.events & EPOLLIN) && (read_size() == 0);
#endif
}

int	Event::read_size( void )
{
	int	count;

#if defined(DARWIN)
	count = _event_os.data;
#endif
#if defined(LINUX)
	ioctl(fd(), FIONREAD, &count);
#endif

	return count;
}



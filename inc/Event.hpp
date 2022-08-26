/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Event.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msousa <mlrcbsousa@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/26 15:52:44 by msousa            #+#    #+#             */
/*   Updated: 2022/08/26 17:01:19 by msousa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __EVENT_H__
# define __EVENT_H__

# include "types.hpp"

// # if defined(DARWIN)
#  define QUEUE() kqueue()
#  define TIMEOUT NULL
// # endif
# if defined(LINUX)
// -->	int epoll_create(int size);
//	The size argument is an indication to the kernel about the number of file descriptors
//	a process wants to monitor, which helps the kernel to decide the size of the epoll
//	instance. Since Linux 2.6.8, this argument is ignored because the epoll data structure
//	dynamically resizes as file descriptors are added or removed from it.
#  define QUEUE() epoll_create(1)
#  define TIMEOUT -1
# endif

// ************************************************************************** //
//                               Event Class                             	  //
// ************************************************************************** //

/*
	Small class to wrap around the OS specific EVENT struct
*/

class Event {

public:

	Event( void );
	Event( Event const & src );
	Event( EVENT const & src );
	~Event( void );
	Event &	operator = ( Event const & rhs );

	/* Getters */
	int		fd( void ) const;
	// int		filter( void ) const;
	// int		flags( void ) const;

private:

	EVENT	_event_os;

};

#endif /* __EVENT_H__ */

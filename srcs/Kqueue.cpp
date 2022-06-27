/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kqueue.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/27 19:11:20 by gleal             #+#    #+#             */
/*   Updated: 2022/06/27 22:28:34 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Kqueue.hpp"
#include <iostream>

Kqueue::CreateError::CreateError( void )
: std::runtime_error("Failed to create Kernel Queue.") { /* No-op */ }

Kqueue::Kqueue()
{
    _fd = kqueue();
    if (_fd < 0)
        throw CreateError();
}

int	Kqueue::getFd() const
{
	return(_fd);
}

void Kqueue::updateEvent(int ident, short filter, u_short flags, u_int fflags, int data, void *udata)
{
    struct kevent kev;
	EV_SET(&kev, ident, filter, flags, fflags, data, udata);
	kevent(this->_fd, &kev, 1, NULL, 0, NULL);
}

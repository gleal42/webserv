/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kqueue.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/27 19:11:20 by gleal             #+#    #+#             */
/*   Updated: 2022/06/29 19:44:23 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Kqueue.hpp"
#include <iostream>

/* 
    Testes a passar:
    Display big images ()
    Vários Servers ao mesmo tempo
    Várias Requests ao mesmo tempo
    EOF working
    HTML CSS priority
    Javascript (later)
 */

Kqueue::CreateError::CreateError( void )
: std::runtime_error("Failed to create Kernel Queue.") { /* No-op */ }

Kqueue::Kqueue()
{
    _fd = kqueue();
    if (_fd < 0)
        throw CreateError();
}

int	Kqueue::fd() const
{
	return(_fd);
}

void Kqueue::update_event(int ident, short filter, u_short flags)
{
    struct kevent kev;
	EV_SET(&kev, ident, filter, flags, 0, 0, NULL);
	kevent(this->_fd, &kev, 1, NULL, 0, NULL);
}

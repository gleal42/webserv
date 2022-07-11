/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kqueue.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/10 19:12:17 by gleal             #+#    #+#             */
/*   Updated: 2022/07/10 21:48:03 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Kqueue.hpp"

void add_event(int kq, int socket_fd, short filter)
{
    struct kevent kev;
	EV_SET(&kev, socket_fd, filter, EV_ADD, 0, 0, NULL);
	kevent(kq, &kev, 1, NULL, 0, NULL);
}

void enable_event(int kq, int socket_fd, short filter)
{
    struct kevent kev;
	EV_SET(&kev, socket_fd, filter, EV_ENABLE, 0, 0, NULL);
	kevent(kq, &kev, 1, NULL, 0, NULL);
}

void disable_event(int kq, int socket_fd, short filter)
{
    struct kevent kev;
	EV_SET(&kev, socket_fd, filter, EV_DISABLE, 0, 0, NULL);
	kevent(kq, &kev, 1, NULL, 0, NULL);
}

void delete_event(int kq, int socket_fd, short filter)
{
    struct kevent kev;
	EV_SET(&kev, socket_fd, filter, EV_DELETE, 0, 0, NULL);
	kevent(kq, &kev, 1, NULL, 0, NULL);
}
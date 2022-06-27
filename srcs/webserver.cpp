/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 19:43:25 by gleal             #+#    #+#             */
/*   Updated: 2022/06/27 23:05:21 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserver.hpp"

#include <algorithm>
#include <arpa/inet.h>

KqueueError::KqueueError( void )
: std::runtime_error("Failed to create Kernel Queue.") { /* No-op */ }

/*
** Creates webserver from configuration (that can handle multiple requests)
** @param:	- [string] argv[1]
** @return:	[int] 0 == SUCCESS
** Line-by-line comments:
** @1	Create Kqueue - Will allow us to identify events and handle
*/

int webserver(std::string input)
{
    ServerConfig config(input);
    Server sv(config);
    Kqueue kq;

    kq.updateEvent(sv.getFd(), EVFILT_READ, EV_ADD, 0, 0, NULL);
    // Setting Server Event Kqueue
    try {
        sv.run(kq);
    }
    catch (const char *str) {
        std::cerr << str << std::endl;
        return (1);
    }
    sv.shutdown();
    return (0);
}

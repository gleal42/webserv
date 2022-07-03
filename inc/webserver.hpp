/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 16:09:11 by msousa            #+#    #+#             */
/*   Updated: 2022/07/01 19:08:08 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERVER_HPP
# define WEBSERVER_HPP

#include <iostream>
#include <string>
#include <sys/event.h>
#include <sys/socket.h>
#include <sys/time.h>

#include "macros.hpp"
#include "Server.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "ConfigParser.hpp"
#include "ServerConfig.hpp"
#include "Socket.hpp"
#include "Kqueue.hpp"
#include "utils.hpp"

struct KqueueError : public std::runtime_error {
	KqueueError( void );
};
# include <string>
# include <cstdlib> // For EXIT_FAILURE, etc, macros
# include <sstream>

# include "macros.hpp"
// Didn't work to include all header files here, if someone wants to give it
// a shot whilst still being able to `make`

// Functions
int 		webserver(std::string config);

#endif

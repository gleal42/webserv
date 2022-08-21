/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmeira <fmeira@student.42lisboa.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 19:43:25 by gleal             #+#    #+#             */
/*   Updated: 2022/08/20 22:35:57 by fmeira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserver.hpp"
#include "ConfigParser.hpp"
#include "ServerConfig.hpp"
#include "Server.hpp"

/*
** Creates webserver from configuration (that can handle multiple requests)
** @param:	- [string] argv[1]
** @return:	[int] 0 == SUCCESS
** Line-by-line comments:
** @1	Create Server - Will allow us to identify events and handle
*/
int webserver(std::string config_file)
{
	ConfigParser	parser(config_file);
	try {
		parser.call();
    }
    catch (std::exception &e) { // Use specific errors
		ERROR(e.what());
    }

	// Initialize all Listeners
    Server webserv(parser);

	// Start waiting for events
	webserv.start();

	// Shutdown and cleanup inside destructor
    return 0;
}


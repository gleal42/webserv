/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 19:43:25 by gleal             #+#    #+#             */
/*   Updated: 2022/07/03 23:24:41 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"
#include "ConfigParser.hpp"
#include "Server.hpp"

#include <algorithm>
#include <arpa/inet.h>

#include "Kqueue.hpp"


/*
** Creates webserver from configuration (that can handle multiple requests)
** @param:	- [string] argv[1]
** @return:	[int] 0 == SUCCESS
** Line-by-line comments:
** @1	Create Kqueue - Will allow us to identify events and handle
*/

int webserver(std::string config_file)
{
	ConfigParser	parser(config_file);
    Kqueue kq;

	try {
        parser.call();
    }
    catch (std::exception &e) { // Use specific errors
		ERROR(e.what());
    }

	// Initialize Cluster
	size_t		amount = parser.configs_amount();
	Server		*new_server;
	Cluster		cluster;
	for (size_t i = 0; i < amount; ++i) {
		// Initialize each new Server with a config from the parser
		ServerConfig	config(parser.config(i));
		new_server = new Server(config);
		kq.update_event(new_server->fd(), EVFILT_READ, EV_ADD);
		cluster[new_server->fd()] = new_server;
	}

	// Start Cluster
	kq.run(cluster);

	// Shutdown and cleanup
	for (ClusterIter it = cluster.begin(); it != cluster.end(); ++it) {
		it->second->shutdown();
		delete it->second;
	}
    return 0;
}


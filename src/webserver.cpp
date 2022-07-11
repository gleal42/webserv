/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 19:43:25 by gleal             #+#    #+#             */
/*   Updated: 2022/07/11 22:39:21 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"
#include "ConfigParser.hpp"
#include "Listener.hpp"
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
	Cluster		cluster;
	Listener		*new_listener;
	for (size_t i = 0; i < amount; ++i) {
		// Initialize each new Listener with a config from the parser
		ServerConfig	config(parser.config(i));
		new_listener = new Listener(config);
		kq.update_event(new_listener->fd(), EVFILT_READ, EV_ADD);
		cluster[new_listener->fd()] = new_listener;
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


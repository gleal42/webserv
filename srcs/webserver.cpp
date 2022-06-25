/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 19:43:25 by gleal             #+#    #+#             */
/*   Updated: 2022/06/25 02:46:52 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"
#include "ConfigParser.hpp"
#include "Server.hpp"

typedef std::vector<Server> Cluster;

int webserver(std::string config_file)
{
	ConfigParser	parser(config_file);
	try {
        parser.call();
    }
    catch (std::exception &e) { // Use specific errors
		ERROR(e.what());
    }

	// Initialize Cluster
	size_t		amount = parser.configs_amount();
	Cluster		cluster(amount);
	for (size_t i = 0; i < amount; ++i) {
		// Initialize each new Server with a config from the parser
		ServerConfig	config(parser.config(i));
		cluster[i] = Server(config);
	}

	// Start Cluster
	for (size_t i = 0; i < cluster.size(); ++i) {
		cluster[i].start();
	}

	// Shutdown and cleanup
	for (size_t i = 0; i < cluster.size(); ++i) {
		cluster[i].shutdown();
	}
    return 0;
}

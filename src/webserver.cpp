/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msousa <mlrcbsousa@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 19:43:25 by gleal             #+#    #+#             */
/*   Updated: 2022/06/25 19:17:59 by msousa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"
#include "ConfigParser.hpp"
#include "Server.hpp"

typedef std::vector<Server*> Cluster;

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
		cluster[i] = new Server(config);
	}

	// Start Cluster
	for (size_t i = 0; i < amount; ++i) {
		cluster[i]->start();
	}

	// Shutdown and cleanup
	for (size_t i = 0; i < amount; ++i) {
		cluster[i]->shutdown();
		delete cluster[i];
	}
    return 0;
}
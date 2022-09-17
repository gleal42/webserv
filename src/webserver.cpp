/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msousa <mlrcbsousa@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 19:43:25 by gleal             #+#    #+#             */
/*   Updated: 2022/09/17 14:02:08 by msousa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "webserver.hpp"

// Global
Server	server;

int	main(int argc, char **argv)
{
	if (argc > 2) {
		ERROR("Wrong number of arguments.");
		return (EXIT_FAILURE);
	}

	if (argc == 1) {
		webserver("default.conf");
		return (EXIT_SUCCESS);
	}

	webserver(std::string(argv[1]));
	return (EXIT_SUCCESS);
}

void	sigint_handler(int sig)
{
	if (sig != SIGINT) {
		return ;
	}

	server.set_sigint(true);
}

/*
** Creates webserver from configuration (that can handle multiple requests)
** @param:	- [string] argv[1]
** @return:	[int] 0 == SUCCESS
** Line-by-line comments:
** @1	Create Server - Will allow us to identify events and handle
*/
int webserver(const std::string &config_file)
{
	ConfigParser	parser(config_file);

	try {
        parser.call();
    }
    catch (ConfigError &e) {
		ERROR("ConfigError; " << e.what());
		exit(EXIT_FAILURE);
    }
    catch (std::exception &e) {
		ERROR(e.what());
		exit(EXIT_FAILURE);
    }

	// Initialize all Listeners
    server.init(parser);

	// Proper cleanup when ctrl-c is pressed
	signal(SIGINT, sigint_handler);

	// Start waiting for events
	try {
        server.start();
    }
    catch (std::exception &e) {
		ERROR("NOT HANDLED PROPERLY: " << e.what());
    }

	// Shutdown and cleanup inside destructor
    return EXIT_SUCCESS;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msousa <mlrcbsousa@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 19:43:25 by gleal             #+#    #+#             */
/*   Updated: 2022/09/07 19:19:25 by msousa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "webserver.hpp"

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
    Server webserv(parser); //TODO: rethink this design

	// Start waiting for events
	webserv.start();

	// Shutdown and cleanup inside destructor
    return EXIT_SUCCESS;
}

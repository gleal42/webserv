/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msousa <mlrcbsousa@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 19:14:39 by gleal             #+#    #+#             */
/*   Updated: 2022/09/06 22:28:52 by msousa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

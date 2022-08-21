/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmeira <fmeira@student.42lisboa.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 19:14:39 by gleal             #+#    #+#             */
/*   Updated: 2022/08/21 01:44:04 by fmeira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserver.hpp"

int	main(int argc, char **argv)
{
	std::cout << argc;
	if (argc != 2)
	{
		ERROR("Wrong number of arguments.");
		return (EXIT_FAILURE);
	}
	else
	{
		webserver(std::string(argv[1])); // Default
		return (EXIT_SUCCESS);
	}
	(void)argv;
	// webserver(argv[1]);
	// webserver("tests/test.conf"); // Temporary stub
	return (EXIT_SUCCESS);
}

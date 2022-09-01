/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 19:14:39 by gleal             #+#    #+#             */
/*   Updated: 2022/09/01 15:05:05 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserver.hpp"

int	main(int argc, char **argv)
{
	if (argc > 2)
	{
		ERROR("Wrong number of arguments.");
		return (EXIT_FAILURE);
	}
	if (argc == 1)
	{
		webserver("test/test.conf"); // Default
		return (EXIT_SUCCESS);
	}
	webserver(std::string(argv[1])); // Default
	(void)argv;
	return (EXIT_SUCCESS);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmeira <fmeira@student.42lisboa.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 19:14:39 by gleal             #+#    #+#             */
/*   Updated: 2022/08/09 20:41:21 by fmeira           ###   ########.fr       */
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
		webserver("tests/test.conf"); // Default
		return (EXIT_SUCCESS);
	}
	(void)argv;
	// webserver(argv[1]);
	webserver("tests/test.conf"); // Temporary stub
	return (EXIT_SUCCESS);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msousa <mlrcbsousa@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 19:14:39 by gleal             #+#    #+#             */
/*   Updated: 2022/06/22 21:00:53 by msousa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserver.hpp"

int	main(int argc, char **argv)
{
	if (argc != 2)
	{
		ERROR("Wrong number of arguments.");
		return (EXIT_FAILURE);
	}
	// webserver(argv[1]);
	(void)argv;
	webserver("tests/test.conf"); // Temporary stub
	return (EXIT_SUCCESS);
}

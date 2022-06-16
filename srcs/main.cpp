/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 19:14:39 by gleal             #+#    #+#             */
/*   Updated: 2022/06/16 04:17:15 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.hpp"

int	main(int argc, char **argv)
{
	if (argc == 2)
	{
		webserver(argv[1]);
		// ERROR("Wrong number of arguments.");
		return (EXIT_FAILURE);
	}
	std::string default_str("tests/test.conf");
	webserver(default_str);
	return (EXIT_SUCCESS);
}

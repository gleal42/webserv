/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 19:14:39 by gleal             #+#    #+#             */
/*   Updated: 2022/06/23 17:26:43by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserver.hpp"

int	main(int argc, char **argv)
{
	if (argc < 2)
	{
		std::string default_str("tests/test.conf");
		return (webserver(default_str));
	}
	else if (argc == 2)
	{
		return (webserver(argv[1]));
	}
	ERROR("Wrong number of arguments.");
	return (EXIT_FAILURE);
}

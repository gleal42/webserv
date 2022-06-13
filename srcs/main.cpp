/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 19:14:39 by gleal             #+#    #+#             */
/*   Updated: 2022/06/06 15:04:00 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.hpp"

int	main(int argc, char **argv)
{
	if (argc != 2)
	{
		webserver(argv[1]);
		// ERROR("Wrong number of arguments.");
		return (EXIT_FAILURE);
	}
	webserver(argv[1]);
	return (EXIT_SUCCESS);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/19 16:18:10 by gleal             #+#    #+#             */
/*   Updated: 2022/08/30 13:47:55 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __PRINT_H__
# define __PRINT_H__

#include <iostream>
#include <vector>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>

template <typename T>
void	print(const T &container)
{
	for (typename T::const_iterator it = container.begin();
		it != container.end();
		it++)
	{
		std::cout << (*it) << std::endl;
	}
}

void	print_env_variables(char *const *envs, size_t size = 0);
void	print_address(const std::string &name, struct sockaddr *address);

#endif /* __PRINT_H__ */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/19 16:18:10 by gleal             #+#    #+#             */
/*   Updated: 2022/08/25 23:33:56 by gleal            ###   ########.fr       */
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

#endif /* __PRINT_H__ */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/19 16:18:10 by gleal             #+#    #+#             */
/*   Updated: 2022/08/19 19:42:56 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __PRINT_H__
# define __PRINT_H__

#include <iostream>

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

void	print(char *const *envs, size_t size = 0);

#endif /* __PRINT_H__ */

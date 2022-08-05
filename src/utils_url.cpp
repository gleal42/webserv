/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_url.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/23 16:55:52 by gleal             #+#    #+#             */
/*   Updated: 2022/08/04 21:47:24 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "utils_url.hpp"

// unsigned char a = 195;
// unsigned char b = 167;
// char a[] = "\xC3";
// char b[] = "\xA7";

namespace url
{
    void	decode( std::string & single_form )
    {
        for (std::string::iterator it = single_form.begin();
            it != single_form.end();)
		{
			if (*it == '%')
			{
				single_form.erase(it);
				*it = str_to_hexa(std::string(it, it + 2));
				single_form.erase(++it);
			}
			else if (*it == '+')
				*it++ = ' ';
			else
				it++;
		}
	}
}

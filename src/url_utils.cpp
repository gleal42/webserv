/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   url_utils.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/23 16:55:52 by gleal             #+#    #+#             */
/*   Updated: 2022/07/24 23:49:28 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "url_utils.hpp"

namespace url
{
	int	str_to_hexa(std::string hexa_nbr)
	{
		std::stringstream ss;
		int x;
		ss << std::hex << hexa_nbr.c_str();
		ss >> x;
		return (x);
	}
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

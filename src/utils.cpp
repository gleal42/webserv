/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/25 19:38:07 by msousa            #+#    #+#             */
/*   Updated: 2022/07/01 18:22:02 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "utils.hpp"

std::string	to_string(int number)
{
	std::string			result;
	std::ostringstream	convert;
	convert << number;
	return convert.str();
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msousa <mlrcbsousa@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/25 19:38:07 by msousa            #+#    #+#             */
/*   Updated: 2022/06/25 19:50:49 by msousa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "webserver.hpp"

std::string	to_string(int number)
{
	std::string			result;
	std::ostringstream	convert;
	convert << number;
	return convert.str();
}

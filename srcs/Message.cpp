/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 01:30:21 by gleal             #+#    #+#             */
/*   Updated: 2022/06/14 01:34:21 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"

const std::string Message::getMessage() const
{
	std::string message(_start_line);
	message += "\n";
	for(size_t i = 0; i < _fields.size(); i++)
	{
		message += _fields[i];
		message += "\n";
	}
	message += "\n";
	message += _body;
	return (message);
}
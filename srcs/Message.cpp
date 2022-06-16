/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 01:30:21 by gleal             #+#    #+#             */
/*   Updated: 2022/06/16 04:52:35 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"

Message::Message()
{}

void Message::addField(std::string field, std::string value)
{
	_fields.insert(std::pair<std::string, std::string>(field, value));
}

void Message::setBody(std::string body)
{
	_body = body;
}

const std::string Message::getBody() const
{
	return (_body);
}

std::map<std::string, std::string> Message::getAllFields() const
{
	return (_fields);
}

std::string Message::getField(std::string field)
{
	std::string fld = _fields[field];
	return (fld);
}


// std::string message(_start_line);
// message += "\n";
// for(size_t i = 0; i < _fields.size(); i++)
// {
// 	message += _fields[i];
// 	message += "\n";
// }
// message += "\n";
// message += _body;
// return (message);
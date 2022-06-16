/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 00:08:17 by gleal             #+#    #+#             */
/*   Updated: 2022/06/16 03:04:31by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
# define MESSAGE_HPP

#include <string>
#include <vector>
#include <map>
#include <utility>


class Message
{
	// Start line (Different for Request and Response)
	// CRLF (Carriage Return)
	std::map<std::string, std::string> _fields; // Different for Request and Response / CRLF (Carriage Return) inbetween them 
	// CRLF (Carriage Return)
	std::string	_body;
	public:
		Message();
		virtual const std::string getStartLine() const = 0;
		void addField(std::string field, std::string value);
		std::map<std::string, std::string> getAllFields() const;
		std::string getField(std::string field);
		const std::string getBody() const;
		void setBody(std::string body);
};

#endif

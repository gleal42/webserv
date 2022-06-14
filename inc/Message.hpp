/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 00:08:17 by gleal             #+#    #+#             */
/*   Updated: 2022/06/14 01:16:50 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
# define MESSAGE_HPP

#include <string>
#include <vector>

class Message
{
	public:
		std::string _start_line; // Different for Request and Response
		// CRLF (Carriage Return)
		std::vector<std::string> _fields; // Different for Request and Response / CRLF (Carriage Return) inbetween them 
		// CRLF (Carriage Return)
		std::string	_body;
		const std::string getMessage() const;
};

#endif

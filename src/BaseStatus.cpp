/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseStatus.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/11 23:25:23 by gleal             #+#    #+#             */
/*   Updated: 2022/07/11 23:27:03 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BaseStatus.hpp"

/* Constructors */
// BaseStatus::BaseStatus( BaseStatus const & src ) { *this = src; }

/* Destructor */
BaseStatus::~BaseStatus( void ) throw () { /* no-op */ } // To be able to subclass

/* Assignment operator */
// BaseStatus &	BaseStatus::operator = ( BaseStatus const & rhs )
// {
// 	if (this != &rhs) {
// 		//value = rhs.value;
// 	}
// 	return *this;
// }

BaseStatus::BaseStatus(
					int code,
					std::string reason_phrase,
					HTTPStatusGroup group )
					: code(code), reason_phrase(reason_phrase), group(group) {}

// Methods
const char*		BaseStatus::what( void ) const throw () {
	return reason_phrase.c_str();
}

bool	BaseStatus::is_status_info( void ) 			{ return http_group(code) == STATUS_INFO; }
bool	BaseStatus::is_status_success( void )		{ return http_group(code) == STATUS_SUCCESS; }
bool	BaseStatus::is_status_redirect( void )		{ return http_group(code) == STATUS_REDIRECT; }
bool	BaseStatus::is_status_error( void ) 		{ return code >= 400; }
bool	BaseStatus::is_status_client_error( void )	{ return http_group(code) == STATUS_CLIENT_ERROR; }
bool	BaseStatus::is_status_server_error( void )	{ return http_group(code) == STATUS_SERVER_ERROR; }

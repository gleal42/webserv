/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseStatus.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msousa <mlrcbsousa@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/14 23:48:22 by msousa            #+#    #+#             */
/*   Updated: 2022/07/14 23:55:00 by msousa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __BASE_STATUS_H__
# define __BASE_STATUS_H__

# include "webserver.hpp"

// ************************************************************************** //
//                               BaseStatus Class                             //
// ************************************************************************** //

class BaseStatus: virtual public std::exception {
	// TODO: canonical form
public:

	// https://stackoverflow.com/questions/121162/what-does-the-explicit-keyword-mean
	explicit BaseStatus( int code,
						 std::string reason_phrase,
						 HTTPStatusGroup group );
	virtual ~BaseStatus( void ) throw ();

	// Variables
	int					code;
	std::string			reason_phrase;
	HTTPStatusGroup		group;

	// Methods
	virtual const char*		what( void ) const throw ();
	bool	is_status_info( void );
	bool	is_status_success( void );
	bool	is_status_redirect( void );
	bool	is_status_error( void );
	bool	is_status_client_error( void );
	bool	is_status_server_error( void );
};

#endif /* __BASE_STATUS_H__ */

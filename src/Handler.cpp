/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Handler.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 17:10:13 by gleal             #+#    #+#             */
/*   Updated: 2022/08/05 01:02:45 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Handler.hpp"



Handler::Handler( void ) { /* no-op */ }
Handler::Handler( Handler const & src ) { *this = src; }

/* Destructor */
Handler::~Handler( void ) { /* no-op */ }

/* Assignment operator */
Handler &	Handler::operator = ( Handler const & rhs )
{
	if (this != &rhs) {
		//value = rhs.value;
	}
	return *this;
}

void	Handler::service( Request & req, Response & res )
{
	if (req.request_method == GET) {
		do_GET(req, res);
	} else if (req.request_method == POST) {
		do_POST(req, res);
	} else if (req.request_method == DELETE) {
		do_DELETE(req, res);
	}

}

void  Handler::do_GET( Request & req, Response & res )
{
	(void)req;
	(void)res;
	throw HTTPStatus<501>();
}

void  Handler::do_POST( Request & req, Response & res )
{
	(void)req;
	(void)res;
	throw HTTPStatus<501>();
}

void  Handler::do_DELETE( Request & req, Response & res )
{
	(void)req;
	(void)res;
	throw HTTPStatus<501>();
}
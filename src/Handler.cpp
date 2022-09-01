/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Handler.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmeira <fmeira@student.42lisboa.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 17:10:13 by gleal             #+#    #+#             */
/*   Updated: 2022/09/01 00:57:17 by fmeira           ###   ########.fr       */
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
	res.build_message(HTTPStatus<200>());
}

void  Handler::do_GET( Request & req, Response & res )
{
	(void)req;
	(void)res;
	throw std::runtime_error("You need to implement `do_GET` when inheriting from Handler!");
}

void  Handler::do_POST( Request & req, Response & res )
{
	(void)req;
	(void)res;
	throw std::runtime_error("You need to implement `do_POST` when inheriting from Handler!");
}

void  Handler::do_DELETE( Request & req, Response & res )
{
	(void)req;
	(void)res;
	throw std::runtime_error("You need to implement `do_DELETE` when inheriting from Handler!");
}

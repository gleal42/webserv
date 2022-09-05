/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Handler.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 17:10:13 by gleal             #+#    #+#             */
/*   Updated: 2022/09/05 22:14:36 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Handler.hpp"

Handler::Handler( void ): _status_code(200){ /* no-op */ }
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

BaseStatus Handler::script_status( void )
{
	if (_status_code == 200)
		return (HTTPStatus<200>());
	if (_status_code == 302)
		return (HTTPStatus<302>());
	if (_status_code == 400)
		return (HTTPStatus<400>());
	if (_status_code == 511)
		return (HTTPStatus<511>());
    if (_status_code == 100)
        return (HTTPStatus<100>());
    if (_status_code == 101)
        return (HTTPStatus<101>());
    if (_status_code == 201)
        return (HTTPStatus<201>());
    if (_status_code == 202)
        return (HTTPStatus<202>());
    if (_status_code == 203)
        return (HTTPStatus<203>());
    if (_status_code == 204)
        return (HTTPStatus<204>());
    if (_status_code == 205)
        return (HTTPStatus<205>());
    if (_status_code == 206)
        return (HTTPStatus<206>());
    if (_status_code == 207)
        return (HTTPStatus<207>());
    if (_status_code == 300)
		return (HTTPStatus<300>());
	if (_status_code == 301)
		return (HTTPStatus<301>());
	if (_status_code == 303)
		return (HTTPStatus<303>());
	if (_status_code == 304)
		return (HTTPStatus<304>());
	if (_status_code == 305)
		return (HTTPStatus<305>());
	if (_status_code == 307)
		return (HTTPStatus<307>());
	if (_status_code == 401)
		return (HTTPStatus<401>());
	if (_status_code == 402)
		return (HTTPStatus<402>());
	if (_status_code == 403)
		return (HTTPStatus<403>());
	if (_status_code == 404)
		return (HTTPStatus<404>());
	if (_status_code == 405)
		return (HTTPStatus<405>());
	if (_status_code == 406)
		return (HTTPStatus<406>());
	if (_status_code == 407)
		return (HTTPStatus<407>());
	if (_status_code == 408)
		return (HTTPStatus<408>());
	if (_status_code == 409)
		return (HTTPStatus<409>());
	if (_status_code == 410)
		return (HTTPStatus<410>());
	if (_status_code == 411)
		return (HTTPStatus<411>());
	if (_status_code == 412)
		return (HTTPStatus<412>());
	if (_status_code == 413)
		return (HTTPStatus<413>());
	if (_status_code == 414)
		return (HTTPStatus<414>());
	if (_status_code == 415)
		return (HTTPStatus<415>());
	if (_status_code == 416)
		return (HTTPStatus<416>());
	if (_status_code == 417)
		return (HTTPStatus<417>());
	if (_status_code == 422)
		return (HTTPStatus<422>());
	if (_status_code == 423)
		return (HTTPStatus<423>());
	if (_status_code == 424)
		return (HTTPStatus<424>());
	if (_status_code == 426)
		return (HTTPStatus<426>());
	if (_status_code == 428)
		return (HTTPStatus<428>());
	if (_status_code == 429)
		return (HTTPStatus<429>());
	if (_status_code == 431)
		return (HTTPStatus<431>());
	if (_status_code == 451)
		return (HTTPStatus<451>());
	if (_status_code == 500)
		return (HTTPStatus<500>());
	if (_status_code == 501)
		return (HTTPStatus<501>());
	if (_status_code == 502)
		return (HTTPStatus<502>());
	if (_status_code == 503)
		return (HTTPStatus<503>());
	if (_status_code == 504)
		return (HTTPStatus<504>());
	if (_status_code == 505)
		return (HTTPStatus<505>());
	if (_status_code == 507)
		return (HTTPStatus<507>());
	return (HTTPStatus<500>());
}

void	Handler::set_status_code( int value )
{
	this->_status_code = value;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Handler.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 17:03:31 by gleal             #+#    #+#             */
/*   Updated: 2022/08/19 22:05:55 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __HANDLER_H__
# define __HANDLER_H__

# include "Request.hpp"
# include "Response.hpp"

class Handler {
public:
	Handler( void );
	Handler( Handler const & src );
	virtual ~Handler( void );
	Handler &	operator = ( Handler const & rhs );
	void	service( Request & req, Response & res );
	BaseStatus	script_status( void );
	void	set_status_code( int value );
private:
	int	_status_code;
	virtual void  do_GET( Request & req, Response & res );
	virtual void  do_POST( Request & req, Response & res );
	virtual void  do_DELETE( Request & req, Response & res );
};

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/12 15:59:39 by gleal             #+#    #+#             */
/*   Updated: 2022/08/31 21:43:30 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __CONNECTION_H__
# define __CONNECTION_H__

# include "Listener.hpp"
# include "Request.hpp"
# include "Response.hpp"
# include "types.hpp"

class Connection : public Socket {

public:

	Connection( void );
	~Connection( void );

	Request		request;
	Response	response;

private:

	Connection( const Connection & src );
	Connection &	operator=( const Connection & src );

};

#endif

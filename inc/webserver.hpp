/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msousa <mlrcbsousa@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 16:09:11 by msousa            #+#    #+#             */
/*   Updated: 2022/07/05 09:30:33 by msousa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERVER_HPP
# define WEBSERVER_HPP

# include <string>
# include <cstdlib> // For EXIT_FAILURE, etc, macros
# include <sstream>
# include <map>

# include "macros.hpp"
// Didn't work to include all header files here, if someone wants to give it
// a shot whilst still being able to `make`

typedef std::map<int, std::string> HTTPStatuses;

enum HTTPStatusGroup {
	STATUS_INVALID,
	STATUS_INFO,
	STATUS_SUCCESS,
	STATUS_REDIRECT,
	STATUS_CLIENT_ERROR,
	STATUS_SERVER_ERROR,
};

// Functions
int 				webserver(std::string config);
std::string			to_string(int number);
std::string const	http_phrase( int code );
HTTPStatusGroup		http_group( int code );

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPStatus.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msousa <mlrcbsousa@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/30 22:26:01 by msousa            #+#    #+#             */
/*   Updated: 2022/08/26 18:11:25 by msousa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __HTTP_STATUS_H__
# define __HTTP_STATUS_H__

# include <iostream>

# include "BaseStatus.hpp"

// ************************************************************************** //
//                            HTTPStatus Template                          	  //
// ************************************************************************** //

template <int Code>
class HTTPStatus: virtual public BaseStatus {
public:
	explicit HTTPStatus( void ) : BaseStatus(Code, http_phrase(Code), http_group(Code)) {}
};

// Output Stream override
template <int Code>
std::ostream &	operator << ( std::ostream & o, HTTPStatus<Code> const & i )
{
	o << i.code << ": " << i.reason_phrase;
	return o;
}

#endif /* __HTTP_STATUS_H__ */

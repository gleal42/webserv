/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_url.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmeira <fmeira@student.42lisboa.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 18:20:46 by gleal             #+#    #+#             */
/*   Updated: 2022/08/21 02:20:04 by fmeira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __UTILS_URL_H__
# define __UTILS_URL_H__

# include <string>
# include <sstream>
# include <iostream>
# include "webserver.hpp"

namespace url
{
    void	decode( std::string & single_form );
}

#endif /* __UTILS_URL_H__ */

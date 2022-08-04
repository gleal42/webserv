/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_url.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/23 16:51:55 by gleal             #+#    #+#             */
/*   Updated: 2022/08/04 19:21:04 by gleal            ###   ########.fr       */
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

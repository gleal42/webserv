/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_url.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmeira <fmeira@student.42lisboa.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/23 16:51:55 by gleal             #+#    #+#             */
/*   Updated: 2022/09/01 00:53:31 by fmeira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __URL_UTILS_H__
# define __URL_UTILS_H__

# include <string>
# include <sstream>
# include <iostream>

# include "webserver.hpp"

namespace url
{
    void	decode( std::string & single_form );
}

#endif /* __URL_UTILS_H__ */

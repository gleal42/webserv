/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   url_utils.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmeira <fmeira@student.42lisboa.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 18:20:46 by gleal             #+#    #+#             */
/*   Updated: 2022/08/05 13:20:35 by fmeira           ###   ########.fr       */
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

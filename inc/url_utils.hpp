/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   url_utils.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/23 16:51:55 by gleal             #+#    #+#             */
/*   Updated: 2022/07/23 17:00:55 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __URL_UTILS_H__
# define __URL_UTILS_H__

# include <string>
# include <sstream>
# include <iostream>

namespace url
{
    void	decode( std::string & single_form );
}

#endif /* __URL_UTILS_H__ */

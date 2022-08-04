/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmeira <fmeira@student.42lisboa.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 18:20:46 by gleal             #+#    #+#             */
/*   Updated: 2022/08/04 17:28:08 by fmeira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __UTILS_H__
# define __UTILS_H__

# include <string>
# include <sstream>

std::string	to_string(int number);
bool is_directory(std::string &path);

#endif

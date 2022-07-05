/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macros.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msousa <mlrcbsousa@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/07 21:17:52 by gleal             #+#    #+#             */
/*   Updated: 2022/07/04 23:03:58 by msousa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MACROS_HPP
# define MACROS_HPP

#include <iostream>

# define LOG(x) std::cout << x << std::endl
# define ERROR(x) std::cerr << x << std::endl

# define CR   "\x0d"		// CR (character : \r, Unicode : U+000D, ASCII : 13, hex : 0x0d)
# define LF   "\x0a"		// LF (character : \n, Unicode : U+000A, ASCII : 10, hex : 0x0a)
# define CRLF "\x0d\x0a"

#endif

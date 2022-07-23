/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   url_utils.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/23 16:55:52 by gleal             #+#    #+#             */
/*   Updated: 2022/07/23 16:59:33 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "url_utils.hpp"

namespace url
{
    void	decode( std::string & single_form )
    {
        std::stringstream processed_url;

        size_t percent = single_form.find('%');
        while (percent != std::string::npos)
        {
            processed_url << single_form.substr(0, percent);
            std::string hexa_nbr = single_form.substr(percent + 1, 2);
            std::stringstream ss;
            unsigned int x;
            ss << std::hex << hexa_nbr.c_str();
            ss >> x;
            unsigned char y = x;
            ss.clear();
            processed_url << y;

            single_form.erase(0, percent+3);
            percent = single_form.find('%');
        }
        processed_url << single_form;
        std::cout << processed_url.str() << std::endl;
        single_form = processed_url.str();

        for (std::string::iterator it = single_form.begin(); it != single_form.end(); it++)
        {
            if (*it == '+')
                *it = ' ';
        }
    }
}

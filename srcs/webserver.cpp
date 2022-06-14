/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 19:43:25 by gleal             #+#    #+#             */
/*   Updated: 2022/06/13 20:06:03 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserver.hpp"

int webserver(char *input)
{
    ServerConfig config(input);

    Server sv(config);

    sv.receive_message();
   
    return 0;
}
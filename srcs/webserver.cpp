/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 19:43:25 by gleal             #+#    #+#             */
/*   Updated: 2022/06/18 00:58:23 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserver.hpp"

int webserver(std::string input)
{
    ServerConfig config(input);
    Server sv(config);

    Request request(sv.getFd(), sv.getAddress()); // Client file descriptor management part
    while (1) // Message Exchange part
    {
        if (request.receive() == EXIT_SUCCESS)
        {
            Response response(config, request);
            response.send(request.getSocket());
        }
    }
    close(request.getSocket());

    return 0;
}
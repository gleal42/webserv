/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 19:43:25 by gleal             #+#    #+#             */
/*   Updated: 2022/06/18 17:57:42 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserver.hpp"

int webserver(std::string input)
{
    ServerConfig config(input);
    Server sv(config);

    try
    {
        Request request(sv.getFd(), &sv.getAddress()); // Client file descriptor management part
        while (1) // Message Exchange part
        {
            request.parse();
            Response response(config, request);
            response.send(request._socket);
        }
        close(request._socket);
    }
    catch (std::string &str)
    {
        std::cerr << str << std::endl;
    }
    return 0;
}
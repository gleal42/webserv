/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 19:43:25 by gleal             #+#    #+#             */
/*   Updated: 2022/06/19 19:57:11 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserver.hpp"

int webserver(std::string input)
{
    ServerConfig config(input);
    Server sv(config);
    try
    {
        int serv_fd = sv.getListeners().begin()->fd;
        Request request(serv_fd, &sv.getAddress()); // Client file descriptor management part
        while (1) // Message Exchange part
        {
            request.parse();
            Response response(config, request);
            response.send(request._socket);
        }
        close(request._socket);
    }
    catch (const char *str)
    {
        std::cerr << str << std::endl;
        close((*sv.getListeners().begin()).fd);
    }
    return 0;
}
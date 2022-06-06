/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 19:05:52 by gleal             #+#    #+#             */
/*   Updated: 2022/06/07 00:46:27 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>
#include <string>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>

# define LOG(x) std::cout << x << std::endl
# define ERROR(x) std::cerr << x << std::endl

typedef struct s_sv_conf
{
    std::string			header;
	int					port;
}                t_sv_conf;

class Server{
private:
	t_sv_conf conf;
	struct sockaddr_in address;
	int fd;
    Server();
	void	init_addr();
public:
    Server(char *config);
    Server(const Server &server);
    ~Server();
    Server &operator=(const Server &server);
	int		receive(void) const;
};

#endif

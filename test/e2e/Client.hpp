/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/07 16:11:28 by gleal             #+#    #+#             */
/*   Updated: 2022/06/07 18:53:13 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <vector>

# define LOG(x) std::cout << x << std::endl
# define ERROR(x) std::cerr << x << std::endl

typedef struct s_sv_conf
{
    std::string			header;
	int					port;
}                t_sv_conf;

class Client 
{
    private:
        t_sv_conf conf;
        struct sockaddr_in address;
        int fd;
        Client();
        Client &operator=(const Client &client);
        Client(const Client &client);
        int	init_addr();
    public:
        Client(const char *config);
        ~Client();
        int		send_msg(void) const;
};
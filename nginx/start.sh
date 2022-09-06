# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    start.sh                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: msousa <mlrcbsousa@gmail.com>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/04/07 00:26:43 by gleal             #+#    #+#              #
#    Updated: 2022/09/06 22:39:49 by msousa           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

chown -R www-data /var/www/*
chmod -R 755 /var/www/*

# cat /etc/nginx/nginx.conf
cp /tmp/default.conf /etc/nginx/nginx.conf
# cp /tmp/default.conf /etc/nginx/sites-available/server.conf
# ln -s /etc/nginx/sites-available/server.conf /etc/nginx/sites-enabled/server.conf

service php7.3-fpm start
service php7.3-fpm status

service nginx start
nginx -t
service nginx status

bash

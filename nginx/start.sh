# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    start.sh                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/04/07 00:26:43 by gleal             #+#    #+#              #
#    Updated: 2022/08/10 21:02:37 by gleal            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

chown -R www-data /var/www/*
chmod -R 755 /var/www/*

cp /tmp/test.conf /etc/nginx/sites-available/server.conf
ln -s /etc/nginx/sites-available/server.conf /etc/nginx/sites-enabled/server.conf

service php7.3-fpm start
service php7.3-fpm status

service nginx start
nginx -t
service nginx status

bash

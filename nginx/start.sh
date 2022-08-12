# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    start.sh                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/04/07 00:26:43 by gleal             #+#    #+#              #
#    Updated: 2022/08/12 03:17:13 by gleal            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

chown -R www-data /var/www/*
chmod -R 755 /var/www/*

service php7.3-fpm start
service php7.3-fpm status

wget 'http://nginx.org/download/nginx-1.22.0.tar.gz'

wget 'https://luajit.org/download/LuaJIT-2.0.5.tar.gz'

wget -O nginx_devel_kit.tar.gz https://github.com/vision5/ngx_devel_kit/archive/refs/tags/v0.3.1.tar.gz

wget -O nginx_lua_module.tar.gz https://github.com/openresty/lua-nginx-module/archive/refs/tags/v0.10.20.tar.gz

tar xvf LuaJIT-2.0.5.tar.gz

tar -xzvf nginx-1.22.0.tar.gz

tar xvf nginx_devel_kit.tar.gz

tar xvf nginx_lua_module.tar.gz

cd /LuaJIT-2.0.5
make install

export LUAJIT_LIB=/usr/local/lib/
export LUAJIT_INC=/usr/local/include/luajit-2.0/

cd /nginx-1.22.0/
# Here we assume you would install you nginx under /opt/nginx/.
./configure --sbin-path=/usr/bin/nginx \
			--pid-path=/var/run/nginx.pid \
			--with-pcre \
			--with-ld-opt="-Wl,-rpath,/usr/local/include/luajit-2.0/" \
			--with-ld-opt="-Wl,-rpath,/usr/local/lib/" \
			--add-module=/ngx_devel_kit-0.3.1/ \
			--add-module=/lua-nginx-module-0.10.20/ \
			--conf-path=/temp/test.conf
make -j2
make install

cd lua-resty-core
make install PREFIX=/

cd lua-resty-lrucache
make install PREFIX=/

# cp /tmp/test.conf /usr/local/nginx/conf/test.conf
# echo "/opt/nginx/lib/lua/?.lua;;"; | cat - /usr/local/nginx/conf/nginx.conf > temp && mv temp /usr/local/nginx/conf/nginx.conf
sed -i "20i     include /usr/local/nginx/conf/test.conf;" /usr/local/nginx/conf/nginx.conf

# ln -s /etc/nginx/sites-available/server.conf /etc/nginx/sites-enabled/server.conf

nginx -t

# service nginx start
# nginx -t
# service nginx status

bash

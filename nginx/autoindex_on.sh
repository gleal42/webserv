rm -rf /etc/nginx/sites-enabled/server.conf
rm -rf /etc/nginx/sites-available/server.conf
cp /tmp/default.conf /etc/nginx/sites-available/server.conf
ln -s /etc/nginx/sites-available/server.conf /etc/nginx/sites-enabled/server.conf
service nginx restart

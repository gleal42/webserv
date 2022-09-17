#!/bin/sh
REQUEST_DATA="first_name=val_1&last_name=val_2&gender=male"
export GATEWAY_INTERFACE="CGI/1.1"
export SERVER_PROTOCOL="HTTP/1.1"
export QUERY_STRING="first_name=other_1&last_name=other_2&gender=male"
export REDIRECT_STATUS="200"
export SCRIPT_FILENAME="best_cgi.php"
export REQUEST_METHOD="POST"
export CONTENT_LENGTH=${#REQUEST_DATA}
export CONTENT_TYPE="application/x-www-form-urlencoded;charset=utf-8"
echo $REQUEST_DATA | /usr/local/bin/php-cgi
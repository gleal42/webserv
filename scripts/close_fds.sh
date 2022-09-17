#!/bin/bash

# database=`lsof -i:$PORT | tr -s ' ' | cut -d ' ' -f 2 | awk 'NR>1'`

database=`lsof -i | grep firefox | grep TCP | grep IPv4| tr -s ' ' | cut -d ' ' -f 2 | awk 'NR>1' | sort -u`
echo open sockets in $database
if [ "$database" ]
then
	kill $database;
fi
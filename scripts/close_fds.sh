#!/bin/bash

database=`lsof -i:8080 | tr -s ' ' | cut -d ' ' -f 2 | awk 'NR>1'`
echo open sockets in 8080: $database
if [ "$database" ]
then
	kill $(lsof -i:8080 | tr -s ' ' | cut -d ' ' -f 2 | awk 'NR>1')
fi
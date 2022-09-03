#!/bin/bash

PORT=8079

database=`lsof -i:$PORT | tr -s ' ' | cut -d ' ' -f 2 | awk 'NR>1'`
echo open sockets in $PORT: $database
if [ "$database" ]
then
	kill $(lsof -i:$PORT | tr -s ' ' | cut -d ' ' -f 2 | awk 'NR>1')
fi
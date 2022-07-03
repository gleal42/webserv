#!/bin/bash

database=`lsof -i:8080 | tr -s ' ' | cut -d ' ' -f 2 | awk 'NR>1'`
if [ -z database ]
then
	kill $(lsof -i:8080 | tr -s ' ' | cut -d ' ' -f 2 | awk 'NR>1') ; \
fi
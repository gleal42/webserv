#!/usr/bin/env bash

set -euo pipefail

if [[ $# > 1 ]]; then
  echo "usage: $0 <config_file>"
  echo
  echo "   <config_file>   Webserv config file"
  exit 1
fi

if [ $# -eq 0 ]; then
  ./webserv
  exit 0
fi

readonly CONFIG_FILE=$1

./webserv ${CONFIG_FILE}

# bash

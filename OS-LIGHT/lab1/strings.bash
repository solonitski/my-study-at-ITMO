#!/bin/bash

while true; do
  read -r line
  if [[ "$line" == "q" ]]; then
    break
  fi
  length=${#line}
  if [[ "$line" =~ ^[[:alpha:]]+$ ]]; then
    match='true'
  else
    match='false'
  fi
  echo "$length" 
  echo "$match"
done

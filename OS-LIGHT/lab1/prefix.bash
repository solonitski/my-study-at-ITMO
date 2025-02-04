#!/bin/bash

if [ $# -ne 2 ]
then
  echo "Please enter two natural numbers as arguments."
  exit 1
fi

sum=0
for ((i=$1; i<=$2; i++))
do
  sum=$((sum + i))
  echo $sum
done

#!/bin/bash

echo $$ > .generator_pid

while true; do
    read input
    echo "GENERATOR: $input"
    echo "$input" > my_pipe
done


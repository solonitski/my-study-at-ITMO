#!/bin/bash

if [ -z "$1" ]; then
    echo "Usage: $0 <N>"
    exit 1
fi

N=$1
ARRAY=()
STEP_COUNT=0

while true; do
    ARRAY+=({1..10})
    ((STEP_COUNT++))

    if [ "${#ARRAY[@]}" -ge "$N" ]; then
        echo "Array reached size $N. Exiting."
        exit 0
    fi
done


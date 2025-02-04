#!/bin/bash

if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <N> <K>"
    exit 1
fi

N=$1
K=$2

echo "Running newmem.bash $K times with N=$N..."

for ((i=0; i<K; i++)); do
    ./newmem.bash "$N" &
    echo "Started process $((i+1)) with PID=$!"
    sleep 1
done

wait

if dmesg | grep -q "newmem.bash"; then
    echo "Some processes failed. Check dmesg for details."
else
    echo "All $K processes completed successfully."
fi


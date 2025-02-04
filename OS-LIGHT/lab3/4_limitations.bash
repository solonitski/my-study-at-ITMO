#!/bin/bash

chmod +x 4_script.bash

./4_script.bash &
pid1=$!
echo "First process started with PID: $pid1"

./4_script.bash &
pid2=$!
echo "Second process started with PID: $pid2"

./4_script.bash &
pid3=$!
echo "Third process started with PID: $pid3"

pid_oldest=$(pgrep -o -f 4_script.bash)
echo "Limiting CPU usage for the process with PID: $pid_oldest"
cpulimit -p "$pid_oldest" -l 10 -b

pid_newest=$(pgrep -n -f 4_script.bash)
echo "Terminating the process with PID: $pid_newest"
kill "$pid_newest"


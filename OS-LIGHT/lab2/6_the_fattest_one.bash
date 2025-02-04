#!/bin/bash

find_max_mem_proc() {
    max_pid=0
    max_mem=0
    max_name=""

    for pid in /proc/[0-9]*; do
        if [ -r "$pid/status" ]; then
            pid_num=$(awk '/^Pid:/ {print $2}' "$pid/status")
            name=$(awk '/^Name:/ {print $2}' "$pid/status")
            mem=$(awk '/^VmRSS:/ {print $2}' "$pid/status")

            if [ -n "$mem" ] && [ "$mem" -gt "$max_mem" ]; then
                max_mem=$mem
                max_pid=$pid_num
                max_name=$name
            fi
        fi
    done

    echo "Process with the highest memory usage via /proc: PID=$max_pid, Name=$max_name"
}

find_max_mem_proc_top() {
    top_mem=$(top -b -o +%MEM | head -n 8 | tail -n 1)
    top_pid=$(echo $top_mem | awk '{print $1}')
    top_name=$(echo $top_mem | awk '{print $12}')
    #top_mem_val=$(echo $top_mem | awk '{print $10}')
    
    echo "Process with the highest memory usage via top: PID=$top_pid, Name=$top_name"
}

find_max_mem_proc
find_max_mem_proc_top

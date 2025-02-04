#!/bin/bash

OUTPUT_FILE="task4.log"

output=""

for pid in /proc/[0-9]*; do
    pid="${pid##*/}"

    if [[ -f "/proc/$pid/status" ]]; then
        ppid=$(awk '/^PPid:/ {print $2}' "/proc/$pid/status")
    fi

    if [[ -f "/proc/$pid/sched" ]]; then
        sum_exec_runtime=$(awk '/se.sum_exec_runtime/ {print $3}' "/proc/$pid/sched")
        nr_switches=$(awk '/nr_switches/ {print $3}' "/proc/$pid/sched")

        if [[ "$nr_switches" -ne 0 ]]; then
            art=$(echo "$sum_exec_runtime / $nr_switches" | bc -l)
            output+="ProcessID=$pid : Parent_ProcessID=$ppid : Average_Running_Time=$art"$'\n'
        fi
    fi
done

echo "$output" | sort -t "=" -k3 -n > "$OUTPUT_FILE"

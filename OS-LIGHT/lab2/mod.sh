#!/bin/bash

if [ $# -lt 2 ]; then
    echo "Usage: $0 <N> <script> [-- <args>*]"
    exit 1
fi

N=$1
shift

SCRIPT=$1
shift

if [ "$1" == "--" ]; then
    shift
fi

ARGS="$@"

cleanup() {
    if kill -0 "$SCRIPT_PID" 2>/dev/null; then
        kill -9 "$SCRIPT_PID" 2>/dev/null
    fi
}
trap cleanup EXIT

"$SCRIPT" $ARGS &
SCRIPT_PID=$!

START_TIME=$(date +%s)

get_descendants() {
    local pid=$1
    local children
    children=$(ps --no-headers --ppid "$pid" -o pid)
    for child in $children; do
        echo "$child"
        get_descendants "$child"
    done
}

get_total_rchar() {
    local pid=$1
    local total=0
    local pids=("$pid")
    
    while IFS= read -r child; do
        pids+=("$child")
    done < <(get_descendants "$pid")
    
    for p in "${pids[@]}"; do
        if [ -r /proc/"$p"/io ]; then
            rchar=$(grep ^rchar /proc/"$p"/io | awk '{print $2}')
            total=$((total + rchar))
        fi
    done
    echo "$total"
}

while true; do
    sleep 1

    if ! kill -0 "$SCRIPT_PID" 2>/dev/null; then
        echo "Script finished before hitting limits."
        exit 0
    fi

    TOTAL_RCHAR=$(get_total_rchar "$SCRIPT_PID")

    if [[ "$TOTAL_RCHAR" =~ ^[0-9]+$ ]] && [ "$TOTAL_RCHAR" -gt "$N" ]; then
        echo "Read limit exceeded: $TOTAL_RCHAR bytes read (limit: $N). Killing script."
        kill -9 "$SCRIPT_PID" 2>/dev/null
        wait "$SCRIPT_PID" 2>/dev/null
        exit 2
    fi

    CURRENT_TIME=$(date +%s)
    ELAPSED=$((CURRENT_TIME - START_TIME))

    if [ "$ELAPSED" -gt 60 ]; then
        echo "Time limit exceeded. Killing script."
        kill -9 "$SCRIPT_PID" 2>/dev/null
        wait "$SCRIPT_PID" 2>/dev/null
        exit 2
    fi
done


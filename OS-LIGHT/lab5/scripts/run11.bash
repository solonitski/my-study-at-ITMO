#!/bin/bash

LOG_DIR="./logs"
mkdir -p $LOG_DIR

> $LOG_DIR/memory_usage.log
> $LOG_DIR/process_usage.log
> $LOG_DIR/system_events.log

./mem.bash &

MEM_BASH_PID=$!

echo "Logging started for mem.bash with PID: $MEM_BASH_PID"

log_system_memory() {
    echo "=== $(date) ===" >> $LOG_DIR/memory_usage.log
    free -m >> $LOG_DIR/memory_usage.log
    echo "" >> $LOG_DIR/memory_usage.log
}

log_process_usage() {
    echo "=== $(date) ===" >> $LOG_DIR/process_usage.log
    ps -p $MEM_BASH_PID -o pid,user,%cpu,%mem,comm >> $LOG_DIR/process_usage.log
    echo "" >> $LOG_DIR/process_usage.log
}

log_system_events() {
    echo "=== $(date) ===" >> $LOG_DIR/system_events.log
    dmesg | grep "mem.bash" >> $LOG_DIR/system_events.log
    echo "" >> $LOG_DIR/system_events.log
}

while kill -0 $MEM_BASH_PID 2>/dev/null; do
    log_system_memory
    log_process_usage
    log_system_events
    sleep 5
done

echo "Process mem.bash has terminated."

log_system_events

echo "Logging completed. Logs are saved in the $LOG_DIR directory."


#!/bin/bash

LOG_DIR="./logs"
mkdir -p $LOG_DIR

> $LOG_DIR/memory_usage_both.log
> $LOG_DIR/process_usage_both.log
> $LOG_DIR/system_events_both.log

./mem.bash &
MEM_BASH_PID=$!
echo "Started mem.bash with PID: $MEM_BASH_PID"

./mem2.bash &
MEM2_BASH_PID=$!
echo "Started mem2.bash with PID: $MEM2_BASH_PID"

log_memory_and_processes() {
    echo "=== $(date) ===" >> $LOG_DIR/memory_usage_both.log
    free -m >> $LOG_DIR/memory_usage_both.log
    echo "" >> $LOG_DIR/memory_usage_both.log

    echo "=== $(date) ===" >> $LOG_DIR/process_usage_both.log
    ps -p $MEM_BASH_PID -o pid,user,%cpu,%mem,comm >> $LOG_DIR/process_usage_both.log 2>/dev/null
    ps -p $MEM2_BASH_PID -o pid,user,%cpu,%mem,comm >> $LOG_DIR/process_usage_both.log 2>/dev/null
    echo "" >> $LOG_DIR/process_usage_both.log

    echo "=== $(date) ===" >> $LOG_DIR/system_events_both.log
    dmesg | tail -n 10 >> $LOG_DIR/system_events_both.log
    echo "" >> $LOG_DIR/system_events_both.log
}

while kill -0 $MEM_BASH_PID 2>/dev/null || kill -0 $MEM2_BASH_PID 2>/dev/null; do
    log_memory_and_processes
    sleep 5
done

echo "Processes terminated. Logs saved to $LOG_DIR."


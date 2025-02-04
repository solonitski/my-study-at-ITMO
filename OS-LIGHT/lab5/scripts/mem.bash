#!/bin/bash

LOG_DIR="./logs"
mkdir -p $LOG_DIR

> $LOG_DIR/report.log

ARRAY=()
STEP_COUNT=0

while true; do
    ARRAY+=({1..10})
    ((STEP_COUNT++))

    if (( STEP_COUNT % 100000 == 0 )); then
        echo "Step: $STEP_COUNT, Array Size: ${#ARRAY[@]}" >> $LOG_DIR/report.log
    fi
done


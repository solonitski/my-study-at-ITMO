#!/bin/bash

value=1

add() {
    value=$((value + 2))
    echo "Handler: Value increased by 2. Current value is $value."
}

multiply() {
    value=$((value * 2))
    echo "Handler: Value multiplied by 2. Current value is $value."
}

terminate() {
    echo "Handler: Received termination signal. Shutting down gracefully."
    exit 0
}

trap add USR1
trap multiply USR2
trap terminate SIGTERM

while true; do
    sleep 1
done


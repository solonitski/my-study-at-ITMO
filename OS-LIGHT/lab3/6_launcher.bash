#!/bin/bash

./6_handler.bash &
handler_pid=$!

if [[ -z "$handler_pid" ]]; then
    echo "Error: Unable to start the handler process."
    exit 1
fi

echo "Handler process started successfully with PID: $handler_pid"

./6_producer.bash $handler_pid


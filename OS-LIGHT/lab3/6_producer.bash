#!/bin/bash

if [[ -z "$1" ]]; then
    echo "Error: Please provide the PID of the handler process as an argument."
    exit 1
fi

handler_pid=$1

if ! kill -0 "$handler_pid" 2>/dev/null; then
    echo "Error: No process found with PID $handler_pid. Ensure the handler is running."
    exit 1
fi

echo "Generator: Ready to process your commands. Type '+' to add, '*' to multiply, or 'TERM' to exit."

while true; do
    read -r line
    case $line in
        "+")
            kill -USR1 "$handler_pid"
            echo "Generator: Sent signal to increase value."
            ;;
        "*")
            kill -USR2 "$handler_pid"
            echo "Generator: Sent signal to multiply value."
            ;;
        "TERM")
            kill -SIGTERM "$handler_pid"
            echo "Generator: Termination signal sent. Exiting."
            exit 0
            ;;
        *)
            echo "Generator: Invalid input. Use '+' to add, '*' to multiply, or 'TERM' to terminate."
            ;;
    esac
done


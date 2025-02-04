#!/bin/bash

result=1
mode=0
generator_pid=$(cat .generator_pid)

tail -f my_pipe | while true; do
    read input
    echo "HANDLER: $input"
    case "$input" in
        "+")
            echo "HANDLER: Switched to addition"
            mode=0
            ;;
        "*")
            echo "HANDLER: Switched to multiplication"
            mode=1
            ;;
        "QUIT")
            echo "HANDLER: Exiting with result: $result"
            kill "$generator_pid"
            exit 0
            ;;
        *)
            if [[ "$input" =~ ^-?[0-9]+$ ]]; then
                if [[ "$mode" -eq 0 ]]; then
                    result=$((result + input))
                else
                    result=$((result * input))
                fi
                echo "HANDLER: Current result: $result"
            else
                echo "HANDLER: Error: Invalid input: $input"
                kill "$generator_pid"
                exit 1
            fi
            ;;
    esac
done


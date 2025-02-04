#!/bin/bash

if [[ "$EUID" -ne 0 ]]; then
    echo "This script must be run as root or with sudo."
    exit 1
fi

initialBytes=""
for entry in /proc/*; do
    pid=$(basename "$entry")
    if [[ "$pid" =~ ^[0-9]+$ ]]; then
        ioFile="$entry/io"
        readBytes=$(sudo awk '/read_bytes/{print $2}' "$ioFile")
        
        initialBytes+="$pid $readBytes\n"
    fi
done

sleep 10

difference=""
for entry in /proc/*; do
    pid=$(basename "$entry")
    if [[ "$pid" =~ ^[0-9]+$ ]]; then
        command=$(cat /proc/$pid/cmdline | tr -d '\0')
        
        ioFile="$entry/io"
        initialValue=$(echo "$initialBytes" | awk '/$pid/{print $2}')
        
        readBytes=$(sudo awk '/read_bytes/{print $2}' "$ioFile")
        delta=$((readBytes - initialValue))
        
        difference+="$pid : $command : $delta\n"
    fi
done

topResults=$(echo -e "$difference" | head -n -1 | sort -n -k5 -r | head -n 3 | sed 's/ : /:/g')
echo -e "$topResults"

#!/bin/bash

echo "MiB Mem : $(free -m | awk '/Mem:/ {print $2}') total"
echo "MiB Swap: $(free -m | awk '/Swap:/ {print $2}') total"
echo "PAGE_SIZE: $(getconf PAGE_SIZE)"
echo "MiB Mem : $(free -m | awk '/Mem:/ {print $7}') free"
echo "MiB Swap: $(free -m | awk '/Swap:/ {print $4}') total"


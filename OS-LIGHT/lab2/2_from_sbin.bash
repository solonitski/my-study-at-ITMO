#!/bin/bash

OUTPUT_FILE="task2.log"

ps aux | grep '/sbin/' | awk '{print $2}' > "$OUTPUT_FILE"

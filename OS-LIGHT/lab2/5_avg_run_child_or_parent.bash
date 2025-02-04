#!/bin/bash

INPUT_FILE="task4.log"
OUTPUT_FILE="task5.log"

count=0
sum=0.0
ppid=-1
res=""

while read -r line; do
    curr=$(echo "$line" | awk -F'Parent_ProcessID=' '{print $2}' | cut -d' ' -f 1)
    art=$(echo "$line" | awk -F'Average_Running_Time=' '{print $2}' | cut -d' ' -f 1)

    if [[ "$ppid" -eq -1 ]]; then
        ppid=$curr
    fi

    if [[ "$curr" -eq "$ppid" ]]; then
        count=$((count + 1))
        sum=$(echo "$sum + $art" | bc -l)
    else
        average=$(echo "$sum / $count" | bc -l)
        res+="Average_Running_Children_of_ParentID=$ppid is $average\n"
        
        sum=$art
        count=1
        ppid=$curr
    fi

    res+="$line\n"

done < "$INPUT_FILE"

average=$(echo "$sum / $count" | bc -l)
res+="Average_Running_Children_of_ParentID=$ppid is $average\n"

echo -e "$res" > "$OUTPUT_FILE"

#!/bin/bash

mkdir -p ~/test
DATE_TIME=$(date '+%Y-%m-%d_%H-%M-%S')
touch ~/test/"$DATE_TIME"

DATE_TIME_MSG=$(date '+%Y-%m-%d:%H-%M-%S')
echo "<$DATE_TIME_MSG> test was created successfully" >> ~/report

mkdir -p ~/test/archived
cd ~/test

dates=$(find . -maxdepth 1 -type f -name '[0-9][0-9][0-9][0-9]-[0-9][0-9]-[0-9][0-9]_*-*-*' ! -name "$DATE_TIME" -printf '%f\n' | cut -d'_' -f1 | sort -u)

for date in $dates; do
    if [[ "$date" < "$(date '+%Y-%m-%d')" ]]; then
        ARCHIVE_NAME="archived/${date}.tar.gz"
        COUNT=1
        while [ -e "$ARCHIVE_NAME" ]; do
            ARCHIVE_NAME="archived/${date}(${COUNT}).tar.gz"
            COUNT=$((COUNT + 1))
        done
        tar -czf "$ARCHIVE_NAME" ${date}_*
        rm ${date}_*
    fi
done

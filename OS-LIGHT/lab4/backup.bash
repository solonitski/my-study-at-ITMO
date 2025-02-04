#!/bin/bash

home="$HOME"
source="$HOME/source"
log="$HOME/backup-report"

date=$(date '+%Y-%m-%d')
found=""

for backup in $(ls "$home"); do
    if ! [[ -d "$home/$backup" ]] || ! [[ "$backup" =~ Backup-[0-9]{4}-[0-9]{2}-[0-9]{2} ]]; then
        continue
    fi
    backup_date=$(echo "$backup" | cut -d'-' -f2-4)
    date_end=$(date -d "$date UTC" +%s)
    date_start=$(date -d "$backup_date UTC" +%s)
    days=$(( ($date_end - $date_start) / (60*60*24) ))

    if (( days < 7 )); then
        found="$backup"
        break
    fi
done

if [[ "$found" == "" ]]; then
    dirname="$home/Backup-$date"
    mkdir "$dirname"
    cp -r "$source/." "$dirname"
    echo "$date: Created backup $dirname" >> "$log"
    find "$source" -type f -exec basename {} \; >> "$log"
else
    echo "$date: Updated backup $home/$found" >> "$log"
    for file in "$source"/*; do
        backup_path="$home/$found/${file#"$source/"}"

        if [[ -d "$file" ]]; then
            if ! [[ -d "$backup_path" ]]; then
                mkdir "$backup_path"
            fi
        else
            if [[ -f "$backup_path" ]]; then
                size1=$(stat --printf="%s" "$file")
                size2=$(stat --printf="%s" "$backup_path")
                if (( size1 != size2 )); then
                    mv "$backup_path" "$backup_path.$date"
                    cp "$file" "$backup_path"
                    echo "Updated: $backup_path (old version: $backup_path.$date)" >> "$log"
                fi
            else
                cp "$file" "$backup_path"
                echo "Added: $backup_path" >> "$log"
            fi
        fi
    done
fi


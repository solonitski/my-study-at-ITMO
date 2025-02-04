#!/bin/bash

backupParentDir="/$HOME/"
restoreDir="/$HOME/restore/"
backupPrefix="Backup-"

mkdir -p "$restoreDir"

latestBackup=$(ls -d "$backupParentDir$backupPrefix"* 2>/dev/null | sort | tail -n 1)

if [[ -z "$latestBackup" ]]; then
    echo "No backup directories found."
    exit 1
fi

echo "Restoring from backup: $latestBackup"

find "$latestBackup" -type f | while read -r file; do
    if [[ "$file" =~ [0-9]{4}-[0-9]{2}-[0-9]{2}$ ]]; then
        continue
    fi

    baseName=$(basename "$file")
    cp "$file" "$restoreDir$baseName"
done

echo "Restore completed."
exit 0


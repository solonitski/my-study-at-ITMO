#!/bin/bash

TRASH_DIR="/$HOME/.trash"
TRASH_LOG="/$HOME/.trash.log"
HOME_DIR="/$HOME"

RESTORE_POLICY="ignore"

usage() {
    echo "Usage: $0 [OPTIONS] filename"
    echo "Restore conflict options:"
    echo "  -i, --ignore    Ignore conflicts (default) - do not restore if the file already exists"
    echo "  -u, --unique    Add a unique suffix to the restored file (file.txt -> file(1).txt)"
    echo "  -o, --overwrite Overwrite the existing file"
    exit 1
}

POSITIONAL_ARGS=()
while [[ $# -gt 0 ]]
do
    case $1 in
        -i|--ignore)
            RESTORE_POLICY="ignore"
            shift
            ;;
        -u|--unique)
            RESTORE_POLICY="unique"
            shift
            ;;
        -o|--overwrite)
            RESTORE_POLICY="overwrite"
            shift
            ;;
        -*)
            echo "Unknown option: $1"
            usage
            ;;
        *)
            POSITIONAL_ARGS+=("$1")
            shift
            ;;
    esac
done

if [[ ${#POSITIONAL_ARGS[@]} -ne 1 ]]; then
    echo "Error: Exactly one filename must be provided."
    usage
fi

FILENAME="${POSITIONAL_ARGS[0]}"

get_unique_name() {
    local base="$1"
    local dir="$2"

    local name_ext="${base##*/}"
    local name="${name_ext%.*}"
    local ext="${name_ext##*.}"
    if [[ "$name_ext" == "$ext" ]]; then
        ext=""
    else
        ext=".$ext"
    fi

    local counter=1
    local new_name="$name($counter)$ext"
    while [[ -e "$dir/$new_name" ]]; do
        ((counter++))
        new_name="$name($counter)$ext"
    done
    echo "$new_name"
}

matches=$(grep -F ":$FILENAME" "$TRASH_LOG" || true)

if [[ -z "$matches" ]]; then
    echo "No records found for file '$FILENAME'."
    exit 0
fi

IFS=$'\n'
for line in $matches; do
    [ -z "$line" ] && continue
    glPath=$(echo "$line" | awk -F ":" '{print $1}')
    trFile=$(echo "$line" | awk -F ":" '{print $2}')

    rmFile="$TRASH_DIR/$trFile"
    if [[ ! -e "$rmFile" ]]; then
        continue
    fi

    read -p "Do you want to restore the file: $glPath ? (y/n) " answer
    if [[ "$answer" != "y" ]]; then
        continue
    fi

    targetDir=$(dirname "$glPath")
    targetName=$(basename "$glPath")
    restorePath="$glPath"

    if [[ ! -d "$targetDir" ]]; then
        echo "Directory $targetDir does not exist! Restoring the file to the home directory."
        targetDir="$HOME_DIR"
        restorePath="$HOME_DIR/$FILENAME"
    fi

    if [[ -e "$restorePath" ]]; then
        case "$RESTORE_POLICY" in
            "ignore")
                echo "File $restorePath already exists. Restoration skipped (ignore)."
                continue
                ;;
            "unique")
                newName=$(get_unique_name "$restorePath" "$targetDir")
                restorePath="$targetDir/$newName"
                ;;
            "overwrite")
                rm -f "$restorePath"
                ;;
        esac
    fi

    ln "$rmFile" "$restorePath"
    if [[ $? -eq 0 ]]; then
        rm "$rmFile"
        sed -i "/${trFile}/d" "$TRASH_LOG"
        echo "File restored to: $restorePath"
    else
        echo "Failed to restore the file: $glPath."
    fi
done


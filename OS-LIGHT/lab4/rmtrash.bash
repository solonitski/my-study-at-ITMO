#!/bin/bash

scriptName=$1
homeTrash="$HOME/.trash"
homeLog="$HOME/.trash.log"

if [[ $# -ne 1 ]]; then
    echo "Error: Only 1 argument is allowed!"
    exit 1
fi

if [[ ! -d $homeTrash ]]; then
    mkdir -p $homeTrash
fi

currentTrashFile="$(date +%s)_$$_$(date +%N | cut -c1-6)"

if [[ -e $scriptName ]]; then
    ln "$scriptName" "$homeTrash/$currentTrashFile"
    rm "$scriptName"

    if [[ ! -e $homeLog ]]; then
        touch $homeLog
    fi

    echo "$PWD/$scriptName:$currentTrashFile" >> $homeLog
    echo "File '$scriptName' moved to trash."
else
    echo "Error: File '$scriptName' does not exist."
    exit 1
fi

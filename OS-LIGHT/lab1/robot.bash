#!/bin/bash

if [ $# -ne 2 ]; then
    echo "Usage: $0 <width> <height>"
    exit 1
fi

if ! [[ "$1" =~ ^[0-9]+$ ]] || ! [[ "$2" =~ ^[0-9]+$ ]]; then
    echo "Width and height should be positive numbers"
    exit 2
fi

width=$1
height=$2

x=$((width / 2))
y=$((height / 2))

function print_position {
    echo "x=$x;y=$y"
}

print_position

while true; do
    read -n 1 -s input

    case $input in
        w|W) ((y++)) ;;
        a|A) ((x--)) ;;
        s|S) ((y--)) ;;
        d|D) ((x++)) ;;
        q|Q) break ;;
        *) continue ;;
    esac

    if [ "$x" -lt 0 ] || [ "$x" -ge "$width" ] || [ "$y" -lt 0 ] || [ "$y" -ge "$height" ]; then
        echo "The robot hit the edge of the field!"
        break
    fi

    print_position
done

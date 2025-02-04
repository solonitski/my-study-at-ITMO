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

declare -A wormholes

function print_position {
    echo "x=$x;y=$y"
}

function add_wormholes {
    read -p "Enter the number of wormholes: " num_wormholes

    for ((i = 1; i <= num_wormholes; i++)); do
        echo "Define wormhole #$i"
        read -p "Enter X coordinate: " wx
        read -p "Enter Y coordinate: " wy
        read -p "Enter direction (w/a/s/d): " dir
        read -p "Enter force (positive integer): " force

        if ! [[ "$wx" =~ ^[0-9]+$ ]] || ! [[ "$wy" =~ ^[0-9]+$ ]] || ! [[ "$force" =~ ^[0-9]+$ ]]; then
            echo "Invalid input. Coordinates and force must be positive integers."
            exit 3
        fi

        if ! [[ "$dir" =~ ^[wasd]$ ]]; then
            echo "Invalid direction. Use one of [w, a, s, d]."
            exit 4
        fi

        wormholes[$wx,$wy]="$dir $force"
    done
}

add_wormholes

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

    if [[ -n ${wormholes[$x,$y]} ]]; then
        IFS=' ' read -r direction force <<< "${wormholes[$x,$y]}"
        case $direction in
            w) ((y += force)) ;;
            a) ((x -= force)) ;;
            s) ((y -= force)) ;;
            d) ((x += force)) ;;
        esac
        echo "The robot encountered a wormhole and was teleported!"
    fi

    if [ "$x" -lt 0 ] || [ "$x" -ge "$width" ] || [ "$y" -lt 0 ] || [ "$y" -ge "$height" ]; then
        echo "The robot hit the edge of the field!"
        break
    fi

    print_position

done


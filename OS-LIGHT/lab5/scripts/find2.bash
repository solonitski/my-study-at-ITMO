#!/bin/bash

check_oom() {
  local N=$1
  local K=$2

  dmesg -C

  for ((i = 0; i < K; i++)); do
    ./newmem.bash "$N" &
    sleep 1

    if dmesg | grep -q 'Out of memory: Killed process'; then
      pkill -P $$
      return 1
    fi
  done

  wait

  if dmesg | grep -q 'Out of memory: Killed process'; then
    return 1
  fi

  return 0
}

binary_search() {
  local K=$1
  local low=100000
  local high=4000000
  local mid=0
  local best=0

  while [ $low -le $high ]; do
    mid=$(((low + high) / 2))
    echo "Testing with n = $mid for $K processes..."

    if check_oom "$mid" "$K"; then
      best=$mid
      low=$((mid + 1))
    else
      high=$((mid - 1))
    fi
  done

  echo "Maximum safe value for N found: $best"
}

if [ -z "$1" ]; then
  echo "Usage: $0 <K>"
  exit 1
fi

binary_search "$1"


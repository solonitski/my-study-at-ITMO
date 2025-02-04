#!/bin/bash

cat /etc/passwd | awk -F: '{print $3, $1}' | sort -k1 -n

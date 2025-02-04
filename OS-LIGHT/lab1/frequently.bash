#!/bin/bash

man bash | tr -s ' ' '\n' | grep -o '[a-zA-Z]\{4,\}' | sort | uniq -c | sort -nr | awk '{print $2}' | head -n 3

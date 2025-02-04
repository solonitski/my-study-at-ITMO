#!/bin/bash

OUTPUT_FILE="task1.log"
USER=$USER

processes=$(ps x -u "${USER}" --no-headers)

echo "${processes}" | wc -l > "$OUTPUT_FILE"
echo "${processes}" | awk '{ print $1, $5 }' >> "$OUTPUT_FILE"

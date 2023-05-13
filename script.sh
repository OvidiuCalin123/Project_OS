#!/bin/bash

if [ $# -eq 0 ]; then
  echo "Usage: $0 <input_file.c>"
  exit 1
fi

input_file=$1

# Compile the C file and filter error and warning messages
error_warnings=$(gcc -o /dev/null -Wall -Wextra $input_file 2>&1 | grep -E 'error|warning')

# Extract the error and warning count
error_count=$(echo "$error_warnings" | grep -c "error")
warning_count=$(echo "$error_warnings" | grep -c "warning")

# Output the error and warning count to a log file
echo "Errors: $error_count" > log.txt
echo "Warnings: $warning_count" >> log.txt

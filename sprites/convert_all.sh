#!/usr/bin/env bash

# Get the directory where this script lives
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Recursively find all .png files from that directory
find "$SCRIPT_DIR" -type f -name "*.png" | while read -r file; do
    echo "Processing: $file"
    python3 "$SCRIPT_DIR/spritesheet-to-rgb332.py" "$file"
done

#!/bin/bash
# Create the output directory if it doesn't exist

# Check if the user provided the maximum number of processors as an argument
if [ $# -lt 1 ]; then
    echo "Usage: $0 <max_processors>"
    exit 1
fi

# Get the maximum number of processors from the user input
max_processors=$1

output_dir="output"
mkdir -p "$output_dir"

# Create the output directory if it doesn't exist
input_dir="input"
mkdir -p "$input_dir"

# Execute gen.sh
echo "Running gen.sh..."
./gen.sh

# Execute avg_time.sh
echo "Running avg_time.sh..."
./avg_time.sh "$max_processors"

# Execute metrics.py using Python
echo "Running metrics.py..."
python3 metrics.py

echo "All scripts have been executed."

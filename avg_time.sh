#!/bin/bash

# Check if the user provided the maximum number of processors as an argument
if [ $# -lt 1 ]; then
    echo "Usage: $0 <max_processors>"
    exit 1
fi

# Get the maximum number of processors from the user input
max_processors=$1

# Define probabilities and their corresponding problem numbers
declare -A problem_map
# problem_map=(["0.1"]="1" ["1"]="2")
problem_map=(["1"]="1")

# Probabilities and matrix sizes
# probabilities=(0.1 1)
probabilities=(1)
sizes=(6000 12000 18000 24000 30000)

# Number of iterations per configuration
iterations=5

# Create the output directory if it doesn't exist
output_dir="output"
mkdir -p "$output_dir"

# Create output file to store average execution times in the output directory
output_file="$output_dir/execution_times.csv"
echo "Problem,Processors,Matrix Size,Average Time" > "$output_file"

# Run tests
for prob in "${probabilities[@]}"; do
    problem=${problem_map[$prob]}
    for size in "${sizes[@]}"; do
        for ((proc = 1; proc <= max_processors; proc++)); do
            total_time=0
            for ((i = 1; i <= iterations; i++)); do
                # Run the MPI program with the current configuration
                mpirun -np "$proc" ./prim "input/$prob/$size.bin"
                
                # Read the execution time from Time.txt
                time=$(cat output/Time.txt)
                total_time=$(echo "$total_time + $time" | bc)
                
                echo "Problem $problem, Processors: $proc, Matrix Size: $size, Iteration $i, Time: $time"
            done
            
            # Calculate the average time
            avg_time=$(echo "scale=6; $total_time / $iterations" | bc)
            
            # Write the result to the output CSV in the output directory
            echo "$problem,$proc,$size,$avg_time" >> "$output_file"
        done
    done
done

echo "All tests completed. Results saved to $output_file."

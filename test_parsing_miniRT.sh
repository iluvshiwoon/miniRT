#!/bin/bash

# Path to your miniRT executable
EXEC="./miniRT"

# Directory containing the test files
TEST_DIR="./rt_test_files"

# Directory to store the outputs
OUTPUT_DIR="./test_outputs"

# Ensure the output directory exists
mkdir -p "$OUTPUT_DIR"

# Iterate over all .rt files in the test directory
for test_file in "$TEST_DIR"/*.rt; do
    # Get the base name of the test file
    base_name=$(basename "$test_file" .rt)

    # Run the miniRT executable with the test file
    "$EXEC" "$test_file" > "$OUTPUT_DIR/$base_name.log" 2>&1

    # Check the exit status of the miniRT executable
    if [ $? -eq 0 ]; then
        echo "$base_name: SUCCESS" >> "$OUTPUT_DIR/results.log"
    else
        echo "$base_name: FAILURE" >> "$OUTPUT_DIR/results.log"
    fi

done

# Print summary of results
echo "Test results summary:" 
cat "$OUTPUT_DIR/results.log"

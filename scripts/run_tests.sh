#!/bin/bash

# Create build directory if it doesn't exist
mkdir -p build

# Create test data directory if it doesn't exist
mkdir -p test_data

# Compile the Google Test suite
g++ -std=c++11 -o build/test_runner \
    tests.cpp \
    src/core/shannon_logic.cpp \
    src/data_structures/frequency_map.cpp \
    src/io/file_handler.cpp \
    -I./ \
    -lgtest -lgtest_main -pthread

# Check if compilation succeeded
if [ $? -ne 0 ]; then
    echo "Failed to compile tests. Please make sure Google Test is installed."
    echo "You can install it with: sudo apt-get install libgtest-dev"
    exit 1
fi

# Run the tests
./build/test_runner

# Get the exit code
exit_code=$?

if [ $exit_code -eq 0 ]; then
    echo "All tests passed successfully!"
else
    echo "Some tests failed. Please check the output above for details."
fi

exit $exit_code 
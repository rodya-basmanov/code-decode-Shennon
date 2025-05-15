#!/bin/bash

# Create build directory if it doesn't exist
mkdir -p build

# Compile the Google Test suite
g++ -std=c++14 -o build/test_runner \
    tests.cpp \
    src/core/shannon_logic.cpp \
    src/data_structures/frequency_map.cpp \
    src/io/file_handler.cpp \
    -I./ \
    -lgtest -lgtest_main -pthread

# Check if compilation succeeded
if [ $? -ne 0 ]; then
    echo "Failed to compile tests"
    exit 1
fi

# Run the tests
./build/test_runner

# Get the exit code
exit_code=$?

if [ $exit_code -eq 0 ]; then
    echo "All tests passed"
else
    echo "Some tests failed"
fi

exit $exit_code 
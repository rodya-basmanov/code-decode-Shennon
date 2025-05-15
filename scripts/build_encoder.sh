#!/bin/bash

# Create build directory if it doesn't exist
mkdir -p build

# Compile the encoder
g++ -std=c++11 -o build/encoder \
    src/app/encoder_entry.cpp \
    src/core/shannon_logic.cpp \
    src/data_structures/frequency_map.cpp \
    src/io/file_handler.cpp \
    -I./src

echo "Encoder compiled successfully. Binary is located at build/encoder" 
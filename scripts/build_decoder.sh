#!/bin/bash

# Create build directory if it doesn't exist
mkdir -p build

# Compile the decoder
g++ -std=c++14 -o build/decoder \
    src/app/decoder_entry.cpp \
    src/core/shannon_logic.cpp \
    src/data_structures/frequency_map.cpp \
    src/io/file_handler.cpp \
    -I./src

echo "Decoder compiled successfully" 
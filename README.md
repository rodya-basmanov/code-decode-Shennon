# Shannon-Fano Coding Project

This project implements the Shannon-Fano coding algorithm for data compression.

## Project Structure

- `src/` - Source code directory
  - `app/` - Application entry points
  - `core/` - Core algorithm implementation
  - `data_structures/` - Data structures for dictionary storage
  - `io/` - File input/output handlers
- `scripts/` - Build and run scripts
- `build/` - Compiled binaries

## Components

### 1. Encoder

Analyzes input binary file, generates a Shannon-Fano coding dictionary, and writes the compressed file along with the dictionary.

### 2. Decoder

Reads the compressed file and dictionary file, and reconstructs the original file.

### 3. Tests

Validates that the decoder correctly reconstructs the original data encoded by the encoder.

## Building

```bash
# Using scripts
./scripts/build_encoder.sh     # Build the encoder
./scripts/build_decoder.sh     # Build the decoder
./scripts/run_tests.sh         # Build and run tests

# Or using Makefile
make encoder                 # Build encoder only
make decoder                 # Build decoder only
make tests                   # Build tests
make run_tests               # Build and run tests
make all                     # Build everything
make clean                   # Clean build files
```

## Usage

### Encoding a file
```bash
./build/encoder <input_file> <output_file> <dictionary_file>
```

### Decoding a file
```bash
./build/decoder <compressed_file> <output_file> <dictionary_file>
```

## Implementation Details

- The dictionary is stored as a mapping between symbols and their corresponding codes.
- The compressed file stores the bit-packed data with padding information.
- The dictionary file format includes the number of entries followed by (symbol, code length, code) for each entry.

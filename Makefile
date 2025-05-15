CXX = g++
CXXFLAGS = -std=c++14 -Wall -Wextra
SRC_DIR = src
BUILD_DIR = build

COMMON_SRC = $(SRC_DIR)/core/shannon_logic.cpp \
             $(SRC_DIR)/data_structures/frequency_map.cpp \
             $(SRC_DIR)/io/file_handler.cpp

all: encoder decoder tests

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Build the encoder
encoder: $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $(BUILD_DIR)/encoder \
		$(SRC_DIR)/app/encoder_entry.cpp \
		$(COMMON_SRC) \
		-I./

# Build the decoder
decoder: $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $(BUILD_DIR)/decoder \
		$(SRC_DIR)/app/decoder_entry.cpp \
		$(COMMON_SRC) \
		-I./

# Build the test suite with Google Test
tests: $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $(BUILD_DIR)/test_runner \
		tests.cpp \
		$(COMMON_SRC) \
		-I./ \
		-lgtest -lgtest_main -pthread

# Run the tests
run_tests: tests
	./$(BUILD_DIR)/test_runner

# Clean build files
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean run_tests encoder decoder tests 
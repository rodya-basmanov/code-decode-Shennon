#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <gtest/gtest.h>

#include "src/core/shannon_logic.h"
#include "src/io/file_handler.h"
#include "src/data_structures/frequency_map.h"

// Helper function to create a test file with sample data
bool create_test_file(const std::string& filename, const std::vector<unsigned char>& data) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to create test file: " << filename << std::endl;
        return false;
    }
    file.write(reinterpret_cast<const char*>(data.data()), data.size());
    return file.good();
}

// Helper function to compare two vectors of bytes
::testing::AssertionResult CompareData(const std::vector<unsigned char>& expected, 
                                     const std::vector<unsigned char>& actual) {
    if (expected.size() != actual.size()) {
        return ::testing::AssertionFailure() 
            << "Size mismatch: expected " << expected.size() 
            << ", got " << actual.size();
    }
    
    for (size_t i = 0; i < expected.size(); i++) {
        if (expected[i] != actual[i]) {
            return ::testing::AssertionFailure() 
                << "Data differs at position " << i 
                << ": expected " << static_cast<int>(expected[i]) 
                << ", got " << static_cast<int>(actual[i]);
        }
    }
    
    return ::testing::AssertionSuccess();
}

// Test fixture for Shannon encoder/decoder tests
class ShannonTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create test directories if they don't exist
        system("mkdir -p build");
    }
    
    void TearDown() override {
        // Clean up test files
        system("rm -f build/*_test_*");
    }
    
    // Test encode and decode with given data
    bool testEncodeDecode(const std::vector<unsigned char>& test_data, const std::string& test_name) {
        std::string input_file = "build/" + test_name + "_input.bin";
        std::string compressed_file = "build/" + test_name + "_compressed.bin";
        std::string dict_file = "build/" + test_name + "_dict.bin";
        std::string output_file = "build/" + test_name + "_output.bin";
        
        // Create test file
        EXPECT_TRUE(create_test_file(input_file, test_data));
        
        // Encode
        EXPECT_TRUE(ShannonCoder::encode(input_file, compressed_file, dict_file));
        
        // Decode
        EXPECT_TRUE(ShannonDecoder::decode(compressed_file, output_file, dict_file));
        
        // Compare input and output
        std::vector<unsigned char> decoded_data = FileHandler::read_binary_file(output_file);
        return CompareData(test_data, decoded_data);
    }
};

// Test with sequential data pattern
TEST_F(ShannonTest, SequentialDataTest) {
    std::vector<unsigned char> test_data;
    for (int i = 0; i < 1000; i++) {
        test_data.push_back(i % 256);
    }
    
    EXPECT_TRUE(testEncodeDecode(test_data, "sequential"));
}

// Test with random data
TEST_F(ShannonTest, RandomDataTest) {
    std::vector<unsigned char> random_data;
    srand(time(nullptr));
    for (int i = 0; i < 5000; i++) {
        random_data.push_back(rand() % 256);
    }
    
    EXPECT_TRUE(testEncodeDecode(random_data, "random"));
}

// Test with a file containing only one byte value
TEST_F(ShannonTest, SingleByteTest) {
    std::vector<unsigned char> single_byte_data(1000, 'A');
    
    EXPECT_TRUE(testEncodeDecode(single_byte_data, "single_byte"));
}

// Test with text data
TEST_F(ShannonTest, TextDataTest) {
    const char* text = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. "
                      "Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. "
                      "Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris "
                      "nisi ut aliquip ex ea commodo consequat.";
    
    std::vector<unsigned char> text_data(text, text + strlen(text));
    
    EXPECT_TRUE(testEncodeDecode(text_data, "text"));
}

// Test dictionary functionality directly
TEST_F(ShannonTest, DictionaryTest) {
    std::vector<unsigned char> test_data;
    for (int i = 0; i < 256; i++) {
        test_data.push_back(i);
    }
    
    FrequencyMap freq_map;
    freq_map.build_from_data(test_data);
    
    std::map<unsigned char, std::string> codes = freq_map.generate_shannon_codes();
    EXPECT_FALSE(codes.empty());
    
    // Save and load dictionary
    std::string dict_file = "build/dict_test.bin";
    EXPECT_TRUE(freq_map.save_to_file(dict_file, codes));
    
    std::map<unsigned char, std::string> loaded_codes = freq_map.load_from_file(dict_file);
    EXPECT_EQ(codes.size(), loaded_codes.size());
    
    // Check each code
    for (const auto& pair : codes) {
        auto it = loaded_codes.find(pair.first);
        EXPECT_TRUE(it != loaded_codes.end());
        if (it != loaded_codes.end()) {
            EXPECT_EQ(pair.second, it->second);
        }
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
} 
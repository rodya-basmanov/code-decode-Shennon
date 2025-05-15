#include <iostream>
#include <string>
#include "../core/shannon_logic.h"

void print_usage() {
    std::cout << "Usage: encoder <input_file> <output_file> <dictionary_file>" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cout << "Error: Invalid number of arguments" << std::endl;
        print_usage();
        return 1;
    }

    std::string input_file = argv[1];
    std::string output_file = argv[2];
    std::string dict_file = argv[3];

    std::cout << "Shannon-Fano Encoder" << std::endl;
    std::cout << "Input file: " << input_file << std::endl;
    std::cout << "Output file: " << output_file << std::endl;
    std::cout << "Dictionary file: " << dict_file << std::endl;

    if (ShannonCoder::encode(input_file, output_file, dict_file)) {
        std::cout << "Encoding completed successfully" << std::endl;
        return 0;
    } else {
        std::cout << "Error occurred during encoding" << std::endl;
        return 1;
    }
} 
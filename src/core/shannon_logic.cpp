#include "shannon_logic.h"
#include "../io/file_handler.h"

std::map<unsigned char, std::string> ShannonCoder::build_codes(const std::vector<unsigned char>& data) {
    FrequencyMap freq_map;
    freq_map.build_from_data(data);
    return freq_map.generate_shannon_codes();
}

bool ShannonCoder::encode(const std::string& input_file, 
                         const std::string& output_file,
                         const std::string& dict_file) {
    std::vector<unsigned char> data = FileHandler::read_binary_file(input_file);
    if (data.empty()) {
        return false;
    }

    std::map<unsigned char, std::string> codes = build_codes(data);
    if (codes.empty()) {
        return false;
    }

    FrequencyMap freq_map;
    if (!freq_map.save_to_file(dict_file, codes)) {
        return false;
    }

    return FileHandler::write_compressed_file(output_file, data, codes);
}

bool ShannonDecoder::decode(const std::string& input_file,
                           const std::string& output_file,
                           const std::string& dict_file) {
    FrequencyMap freq_map;
    std::map<unsigned char, std::string> codes = freq_map.load_from_file(dict_file);
    if (codes.empty()) {
        return false;
    }

    std::vector<unsigned char> decoded_data = FileHandler::read_compressed_file(input_file, codes);
    if (decoded_data.empty()) {
        return false;
    }

    return FileHandler::write_binary_file(output_file, decoded_data);
} 
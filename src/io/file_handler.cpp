#include "file_handler.h"
#include <fstream>
#include <bitset>

std::vector<unsigned char> FileHandler::read_binary_file(const std::string& file_path) {
    std::ifstream file(file_path, std::ios::binary);
    if (!file) {
        return {};
    }

    file.seekg(0, std::ios::end);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<unsigned char> buffer(size);
    if (size > 0) {
        file.read(reinterpret_cast<char*>(buffer.data()), size);
    }

    return buffer;
}

bool FileHandler::write_binary_file(const std::string& file_path, const std::vector<unsigned char>& data) {
    std::ofstream file(file_path, std::ios::binary);
    if (!file) {
        return false;
    }

    if (!data.empty()) {
        file.write(reinterpret_cast<const char*>(data.data()), data.size());
    }
    
    return file.good();
}

bool FileHandler::write_compressed_file(const std::string& file_path, 
                                       const std::vector<unsigned char>& data,
                                       const std::map<unsigned char, std::string>& codes) {
    if (data.empty() || codes.empty()) {
        return false;
    }

    std::ofstream file(file_path, std::ios::binary);
    if (!file) {
        return false;
    }

    std::string bit_stream;
    for (unsigned char byte : data) {
        auto it = codes.find(byte);
        if (it == codes.end()) {
            return false;
        }
        bit_stream += it->second;
    }

    int padding = 8 - (bit_stream.length() % 8);
    if (padding == 8) padding = 0;

    file.write(reinterpret_cast<const char*>(&padding), sizeof(padding));

    std::vector<unsigned char> compressed;
    for (size_t i = 0; i < bit_stream.length(); i += 8) {
        std::string byte_str = bit_stream.substr(i, 8);
        while (byte_str.length() < 8) {
            byte_str += '0';
        }
        unsigned char byte = 0;
        for (int j = 0; j < 8; j++) {
            if (byte_str[j] == '1') {
                byte |= (1 << (7 - j));
            }
        }
        compressed.push_back(byte);
    }

    file.write(reinterpret_cast<const char*>(compressed.data()), compressed.size());
    
    return file.good();
}

std::vector<unsigned char> FileHandler::read_compressed_file(const std::string& file_path,
                                                          const std::map<unsigned char, std::string>& codes) {
    std::ifstream file(file_path, std::ios::binary);
    if (!file) {
        return {};
    }

    std::map<std::string, unsigned char> decode_map;
    for (auto const& pair : codes) {
        unsigned char symbol = pair.first;
        std::string code = pair.second;
        decode_map[code] = symbol;
    }

    int padding;
    file.read(reinterpret_cast<char*>(&padding), sizeof(padding));
    if (!file.good()) {
        return {};
    }

    std::vector<unsigned char> compressed;
    while (file) {
        unsigned char byte;
        file.read(reinterpret_cast<char*>(&byte), 1);
        if (file.gcount() == 1) {
            compressed.push_back(byte);
        }
    }

    std::string bit_stream;
    for (size_t i = 0; i < compressed.size(); i++) {
        std::bitset<8> bits(compressed[i]);
        bit_stream += bits.to_string();
    }

    if (padding > 0 && bit_stream.length() >= padding) {
        bit_stream = bit_stream.substr(0, bit_stream.length() - padding);
    }

    std::vector<unsigned char> decoded;
    std::string current_code;
    for (char bit : bit_stream) {
        current_code += bit;
        auto it = decode_map.find(current_code);
        if (it != decode_map.end()) {
            decoded.push_back(it->second);
            current_code.clear();
        }
    }

    return decoded;
} 
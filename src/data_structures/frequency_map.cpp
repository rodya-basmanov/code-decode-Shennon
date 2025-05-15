#include "frequency_map.h"
#include <fstream>
#include <algorithm>
#include <cmath>

void FrequencyMap::build_from_data(const std::vector<unsigned char>& data) {
    std::map<unsigned char, int> counts;
    for (unsigned char byte : data) {
        counts[byte]++;
    }

    frequencies.clear();
    for (auto const& pair : counts) {
        frequencies.push_back({pair.first, pair.second});
    }
    std::sort(frequencies.begin(), frequencies.end());
}

void assign_codes_recursive(std::vector<SymbolFrequency>& symbols, 
                            std::map<unsigned char, std::string>& codes,
                            const std::string& current_code) {
    if (symbols.empty()) {
        return;
    }

    if (symbols.size() == 1) {
        codes[symbols[0].symbol] = current_code.empty() ? "0" : current_code;
        return;
    }

    long long total_frequency = 0;
    for(const auto& sf : symbols) {
        total_frequency += sf.frequency;
    }

    long long current_sum = 0;
    int split_index = 0;
    long long min_diff = -1;

    for (size_t i = 0; i < symbols.size() - 1; ++i) {
        current_sum += symbols[i].frequency;
        long long diff = std::abs(current_sum - (total_frequency - current_sum));
        if (min_diff == -1 || diff < min_diff) {
            min_diff = diff;
            split_index = i;
        }
    }
    
    std::vector<SymbolFrequency> group1(symbols.begin(), symbols.begin() + split_index + 1);
    std::vector<SymbolFrequency> group2(symbols.begin() + split_index + 1, symbols.end());

    assign_codes_recursive(group1, codes, current_code + "0");
    assign_codes_recursive(group2, codes, current_code + "1");
}

std::map<unsigned char, std::string> FrequencyMap::generate_shannon_codes() {
    std::map<unsigned char, std::string> codes_map;
    if (frequencies.empty()) {
        return codes_map;
    }
    
    std::vector<SymbolFrequency> sorted_symbols = frequencies; 
    std::sort(sorted_symbols.begin(), sorted_symbols.end(), [](const SymbolFrequency& a, const SymbolFrequency& b){
        return a.frequency > b.frequency;
    });

    assign_codes_recursive(sorted_symbols, codes_map, "");
    return codes_map;
}

bool FrequencyMap::save_to_file(const std::string& file_path, const std::map<unsigned char, std::string>& codes) const {
    std::ofstream out_file(file_path, std::ios::binary);
    if (!out_file) {
        return false;
    }

    int num_entries = codes.size();
    out_file.write(reinterpret_cast<const char*>(&num_entries), sizeof(num_entries));

    for (auto const& pair : codes) {
        unsigned char symbol = pair.first;
        std::string code_str = pair.second;
        out_file.write(reinterpret_cast<const char*>(&symbol), sizeof(symbol));
        int code_len = code_str.length();
        out_file.write(reinterpret_cast<const char*>(&code_len), sizeof(code_len));
        out_file.write(code_str.c_str(), code_len);
    }

    out_file.close();
    return true;
}

std::map<unsigned char, std::string> FrequencyMap::load_from_file(const std::string& file_path) {
    std::map<unsigned char, std::string> codes_map;
    std::ifstream in_file(file_path, std::ios::binary);
    if (!in_file) {
        return codes_map;
    }

    int num_entries;
    in_file.read(reinterpret_cast<char*>(&num_entries), sizeof(num_entries));
    if (in_file.gcount() != sizeof(num_entries)) return codes_map;

    for (int i = 0; i < num_entries; ++i) {
        unsigned char symbol;
        in_file.read(reinterpret_cast<char*>(&symbol), sizeof(symbol));
        if (in_file.gcount() != sizeof(symbol)) return codes_map; 

        int code_len;
        in_file.read(reinterpret_cast<char*>(&code_len), sizeof(code_len));
        if (in_file.gcount() != sizeof(code_len)) return codes_map; 

        if (code_len < 0 || code_len > 256) {
            return codes_map;
        }

        std::vector<char> code_buffer(code_len);
        in_file.read(code_buffer.data(), code_len);
        if (in_file.gcount() != code_len) return codes_map; 
        
        codes_map[symbol] = std::string(code_buffer.begin(), code_buffer.end());
    }

    in_file.close();
    return codes_map;
}

const std::vector<SymbolFrequency>& FrequencyMap::get_frequencies() const {
    return frequencies;
} 
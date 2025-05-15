#ifndef FREQUENCY_MAP_H
#define FREQUENCY_MAP_H

#include <string>
#include <vector>
#include <map>

// Structure to store symbol-code pair
struct SymbolCode {
    unsigned char symbol;
    std::string code;
};

// Structure to store symbol frequency
struct SymbolFrequency {
    unsigned char symbol;
    int frequency;
    // For sorting
    bool operator<(const SymbolFrequency& other) const {
        return frequency > other.frequency; // Sort by descending frequency
    }
};

class FrequencyMap {
public:
    // Count symbol frequencies from input data
    void build_from_data(const std::vector<unsigned char>& data);

    // Generate Shannon codes based on frequencies
    // Returns a map of symbol -> code
    std::map<unsigned char, std::string> generate_shannon_codes();

    // Save dictionary (symbol-code map) to file
    bool save_to_file(const std::string& file_path, const std::map<unsigned char, std::string>& codes) const;

    // Load dictionary (symbol-code map) from file
    std::map<unsigned char, std::string> load_from_file(const std::string& file_path);

    // Get frequency table (for code generation)
    const std::vector<SymbolFrequency>& get_frequencies() const;

private:
    std::vector<SymbolFrequency> frequencies;
};

#endif // FREQUENCY_MAP_H 
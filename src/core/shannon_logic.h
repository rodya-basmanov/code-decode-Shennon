#ifndef SHANNON_LOGIC_H
#define SHANNON_LOGIC_H

#include <string>
#include <vector>
#include <map>
#include "../data_structures/frequency_map.h"

class ShannonCoder {
public:
    // Encodes input data using Shannon-Fano algorithm
    static bool encode(const std::string& input_file, 
                      const std::string& output_file,
                      const std::string& dict_file);

private:
    // Internal logic for encoding
    static std::map<unsigned char, std::string> build_codes(const std::vector<unsigned char>& data);
};

class ShannonDecoder {
public:
    // Decodes compressed data using dictionary file
    static bool decode(const std::string& input_file,
                      const std::string& output_file,
                      const std::string& dict_file);
};

#endif // SHANNON_LOGIC_H 
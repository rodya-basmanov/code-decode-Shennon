#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <string>
#include <vector>
#include <map>

class FileHandler {
public:
    // Reads binary file and returns content as byte vector
    static std::vector<unsigned char> read_binary_file(const std::string& file_path);
    
    // Writes bytes to binary file
    static bool write_binary_file(const std::string& file_path, const std::vector<unsigned char>& data);
    
    // Writes compressed data using provided codes
    static bool write_compressed_file(const std::string& file_path, 
                                     const std::vector<unsigned char>& data,
                                     const std::map<unsigned char, std::string>& codes);
    
    // Reads compressed file and decodes using provided codes
    static std::vector<unsigned char> read_compressed_file(const std::string& file_path,
                                                         const std::map<unsigned char, std::string>& codes);
};

#endif // FILE_HANDLER_H 
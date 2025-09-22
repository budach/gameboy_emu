#include "helpers.h"
#include <fstream>
#include <vector>
#include <string>
#include <iostream>

std::vector<uint8_t> read_rom(const std::string &filename)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file)
    {
        std::cerr << "Failed to open ROM file: " << filename << std::endl;
        exit(1);
    }

    std::vector<uint8_t> buffer(std::istreambuf_iterator<char>(file), {});
    return buffer;
}

#include "mmu.h"
#include <algorithm>
#include <iterator>
#include <iostream>
#include <fstream>

MMU::MMU()
{
    mem.resize(MMU_ADDRESSABLE_MEM, 0);
}

void MMU::load_boot_rom(const std::string &filename)
{
    std::ifstream file(filename, std::ios::binary);

    if (!file)
    {
        std::cerr << "Failed to open ROM file: " << filename << std::endl;
        exit(1);
    }

    std::vector<uint8_t> buffer(std::istreambuf_iterator<char>(file), {});

    if (buffer.size() != 256)
    {
        std::cerr << "Boot ROM must be exactly 256 bytes" << std::endl;
        exit(1);
    }

    std::copy(buffer.begin(), buffer.end(), mem.begin());
}

uint8_t MMU::read8(uint16_t address) const
{
    return mem[address];
}

void MMU::write8(uint16_t address, uint8_t value)
{
    mem[address] = value;
}

#pragma once

#include <cstdint>
#include <vector>
#include <string>

const size_t MMU_ADDRESSABLE_MEM = 0x10000; // 64KB

struct MMU
{
    std::vector<uint8_t> mem;

    MMU();

    void load_boot_rom(const std::string &filename);

    uint8_t read8(uint16_t address) const;
    void write8(uint16_t address, uint8_t value);
};

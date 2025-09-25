#pragma once

#include <cstdint>
#include <string>
#include <vector>

const size_t MMU_ADDRESSABLE_MEM = 0x10000; // 64KB
const size_t MMU_BOOT_ROM_SIZE = 0x0100;    // 256 bytes

struct MMU
{
    std::vector<uint8_t> mem;
    std::vector<uint8_t> boot_rom_mem;
    bool boot_rom_active;

    MMU();

    void load_boot_rom(const std::string &filename);
    void load_game_rom(const std::string &filename);

    uint8_t read8(uint16_t address) const;
    uint16_t read16(uint16_t address) const;
    void write8(uint16_t address, uint8_t value);
    void write16(uint16_t address, uint16_t value);
};

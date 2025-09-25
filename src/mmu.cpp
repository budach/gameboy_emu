#include "mmu.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>

MMU::MMU()
{
    mem.resize(MMU_ADDRESSABLE_MEM, 0);
    boot_rom_mem.resize(MMU_BOOT_ROM_SIZE, 0);
    boot_rom_active = false;
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

    std::copy(buffer.begin(), buffer.end(), boot_rom_mem.begin());
    boot_rom_active = true;
}

void MMU::load_game_rom(const std::string &filename)
{
    std::ifstream file(filename, std::ios::binary);

    if (!file)
    {
        std::cerr << "Failed to open game ROM file: " << filename << std::endl;
        exit(1);
    }

    std::vector<uint8_t> buffer(std::istreambuf_iterator<char>(file), {});

    if (buffer.size() > 0x8000)
    {
        std::cerr << "Game ROM too large (max 32KB for now)" << std::endl;
        exit(1);
    }

    std::copy(buffer.begin(), buffer.end(), mem.begin());
}

uint8_t MMU::read8(uint16_t address) const
{
    if (boot_rom_active && address < MMU_BOOT_ROM_SIZE)
    {
        return boot_rom_mem[address];
    }

    std::cout << "Reading from game at address: 0x" << std::hex << address
              << std::dec << std::endl;
    return mem[address];
}

uint16_t MMU::read16(uint16_t address) const
{
    return (read8(address + 1) << 8) | read8(address);
}

void MMU::write8(uint16_t address, uint8_t value)
{
    if (address == 0xFF50 && boot_rom_active)
    {
        boot_rom_active = false;
        return;
    }

    if (boot_rom_active && address < MMU_BOOT_ROM_SIZE)
    {
        std::cout << "Writing to boot ROM address 0x" << std::hex << address
                  << std::dec << std::endl;
        exit(1);
    }

    mem[address] = value;
}

void MMU::write16(uint16_t address, uint16_t value)
{
    write8(address, value & 0xFF);            // low byte
    write8(address + 1, (value >> 8) & 0xFF); // high byte
}

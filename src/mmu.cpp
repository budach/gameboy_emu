#include "mmu.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>

MMU::MMU()
{
    mem.resize(MMU_ADDRESSABLE_MEM, 0);

    // set hardware registers to initial values after boot ROM execution
    // from https://gbdev.io/pandocs/Power_Up_Sequence.html
    mem[0xFF00] = 0xCF;
    mem[0xFF01] = 0x00;
    mem[0xFF02] = 0x7E;
    mem[0xFF04] = 0xAB;
    mem[0xFF05] = 0x00;
    mem[0xFF06] = 0x00;
    mem[0xFF07] = 0xF8;
    mem[0xFF0F] = 0xE1;
    mem[0xFF10] = 0x80;
    mem[0xFF11] = 0xBF;
    mem[0xFF12] = 0xF3;
    mem[0xFF13] = 0xFF;
    mem[0xFF14] = 0xBF;
    mem[0xFF16] = 0x3F;
    mem[0xFF17] = 0x00;
    mem[0xFF18] = 0xFF;
    mem[0xFF19] = 0xBF;
    mem[0xFF1A] = 0x7F;
    mem[0xFF1B] = 0xFF;
    mem[0xFF1C] = 0x9F;
    mem[0xFF1D] = 0xFF;
    mem[0xFF1E] = 0xBF;
    mem[0xFF20] = 0xFF;
    mem[0xFF21] = 0x00;
    mem[0xFF22] = 0x00;
    mem[0xFF23] = 0xBF;
    mem[0xFF24] = 0x77;
    mem[0xFF25] = 0xF3;
    mem[0xFF26] = 0xF1;
    mem[0xFF40] = 0x91;
    mem[0xFF41] = 0x85;
    mem[0xFF42] = 0x00;
    mem[0xFF43] = 0x00;
    mem[0xFF44] = 0x00;
    mem[0xFF45] = 0x00;
    mem[0xFF46] = 0xFF;
    mem[0xFF47] = 0xFC;
    mem[0xFF48] = 0x00;
    mem[0xFF49] = 0x00;
    mem[0xFF4A] = 0x00;
    mem[0xFF4B] = 0x00;
    mem[0xFFFF] = 0x00;
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
    return mem[address];
}

uint16_t MMU::read16(uint16_t address) const
{
    return (read8(address + 1) << 8) | read8(address);
}

void MMU::write8(uint16_t address, uint8_t value)
{
    mem[address] = value;
}

void MMU::write16(uint16_t address, uint16_t value)
{
    write8(address, value & 0xFF);            // low byte
    write8(address + 1, (value >> 8) & 0xFF); // high byte
}

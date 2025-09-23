#pragma once

#include <cstdint>
#include <string>

#include "opcodes.h"
#include "mmu.h"

const size_t GB_NUM_OPCODES = 256;

struct Gameboy
{
    Opcode opcode_table[GB_NUM_OPCODES]; // opcode lookup table
    MMU mmu;                             // memory management unit
    uint16_t SP;                         // stack pointer
    uint16_t PC;                         // program counter

    // registers
    union
    {
        uint16_t AF;
        struct
        {
            uint8_t F; // low
            uint8_t A; // high
        } AF_bytes;
    };

    // flag bits in the F register:
    // bit 7 - Z - zero Flag
    // bit 6 - N - subtract Flag
    // bit 5 - H - half Carry Flag
    // bit 4 - C - carry Flag

    union
    {
        uint16_t BC;
        struct
        {
            uint8_t C; // low
            uint8_t B; // high
        } BC_bytes;
    };

    union
    {
        uint16_t DE;
        struct
        {
            uint8_t E; // low
            uint8_t D; // high
        } DE_bytes;
    };

    union
    {
        uint16_t HL;
        struct
        {
            uint8_t L; // low
            uint8_t H; // high
        } HL_bytes;
    };

    Gameboy(const std::string &boot_rom_filename);

    void run_cycle();
};

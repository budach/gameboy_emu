#pragma once

#include <cstdint>
#include <string>

#include "mmu.h"
#include "opcodes.h"

// 256 "normal" opcodes and 256 CB-prefixed opcodes = 512 total
const size_t GB_NUM_OPCODES = 256;

// flag bit masks for the F register
constexpr uint8_t GB_FLAG_Z = 1 << 7; // zero flag
constexpr uint8_t GB_FLAG_N = 1 << 6; // subtract flag
constexpr uint8_t GB_FLAG_H = 1 << 5; // half Carry flag
constexpr uint8_t GB_FLAG_C = 1 << 4; // carry flag

struct Gameboy
{
    uint8_t (*opcodes[GB_NUM_OPCODES])(Gameboy &);    // opcode lookup table
    uint8_t (*cb_opcodes[GB_NUM_OPCODES])(Gameboy &); // CB-prefixed opcode lookup table
    MMU mmu;                                          // memory management unit
    uint16_t SP;                                      // stack pointer
    uint16_t PC;                                      // program counter

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

    Gameboy(const std::string &boot_rom_filename, const std::string &game_rom_filename);

    uint8_t run_opcode();
};

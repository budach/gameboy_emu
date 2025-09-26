#pragma once

#include <cstdint>
#include <string>

#include "mmu.h"
#include "opcodes.h"
#include "cpu.h"
#include "ppu.h"

// 256 "normal" opcodes and 256 CB-prefixed opcodes = 512 total
const size_t GB_NUM_OPCODES = 256;

struct Gameboy
{
    uint8_t (*opcodes[GB_NUM_OPCODES])(Gameboy &);    // opcode lookup table
    uint8_t (*cb_opcodes[GB_NUM_OPCODES])(Gameboy &); // CB-prefixed opcode lookup table
    MMU mmu;                                          // memory management unit
    CPU cpu;                                          // CPU registers and state
    PPU ppu;                                          // pixel processing unit

    Gameboy(const std::string &game_rom_filename);

    uint8_t run_opcode();
};

#include <iostream>

#include "gameboy.h"
#include "helpers.h"
#include "opcode.h"

Gameboy::Gameboy(const std::string &rom_filename)
{
    // init registers

    AF = 0;
    BC = 0;
    DE = 0;
    HL = 0;

    // init stack pointer and program counter

    SP = 0;
    PC = 0;

    // init memory

    mem.resize(GB_ADDRESSABLE_MEM, 0);
    auto rom = read_rom(rom_filename);

    if (rom.size() > GB_ADDRESSABLE_MEM)
    {
        std::cerr << "ROM size exceeds addressable memory" << std::endl;
        exit(1);
    }

    std::copy(rom.begin(), rom.end(), mem.begin());

    // init opcodes

    for (size_t i = 0; i < GB_NUM_OPCODES; i++)
    {
        opcode_table[i] = {"UNIMPLEMENTED", op_unimplemented, 0, 0, 0};
    }

    opcode_table[0x21] = {"LD HL,u16", op_0x21_LD_HL_u16, 3, 12, 3};
    opcode_table[0x31] = {"LD SP,d16", op_0x31_LD_SP_u16, 3, 12, 3};
    opcode_table[0x32] = {"LD (HL-),A", op_0x32_LD_HLm_A, 1, 8, 2};
    opcode_table[0xAF] = {"XOR A,A", op_0xAF_XOR_A_A, 1, 4, 1};
}

void Gameboy::run_cycle()
{
    uint8_t opcode = read_mem(PC);

    std::cout << "Executing opcode: 0x" << std::hex << (int)opcode
              << " - " << opcode_table[opcode].name
              << " at PC: " << PC << std::dec << std::endl;

    opcode_table[opcode].execute(*this);
    PC += opcode_table[opcode].length;
}

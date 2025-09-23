#include "opcodes.h"
#include "gameboy.h"

#include <iostream>

void op_0x21_LD_HL_u16(Gameboy &gb)
{
    // set HL to the 16-bit value following the opcode
    gb.HL = gb.mmu.read8(gb.PC + 1) | (gb.mmu.read8(gb.PC + 2) << 8);
}

void op_0x31_LD_SP_u16(Gameboy &gb)
{
    // set SP to the 16-bit value following the opcode
    gb.SP = gb.mmu.read8(gb.PC + 1) | (gb.mmu.read8(gb.PC + 2) << 8);
}

void op_0x32_LD_HLm_A(Gameboy &gb)
{
    // store A into memory at address HL, then decrement HL
    gb.mmu.write8(gb.HL, gb.AF_bytes.A);
    gb.HL--;
}

void op_0xAF_XOR_A_A(Gameboy &gb)
{
    gb.AF_bytes.A ^= gb.AF_bytes.A; // result is always 0
    gb.AF_bytes.F = 0x80;           // Z flag set (bit 7), others cleared
}

void op_unimplemented(Gameboy &gb)
{
    std::cerr << "Unimplemented opcode: 0x" << std::hex << (int)gb.mmu.read8(gb.PC)
              << " at PC: " << gb.PC << std::dec << std::endl;
    exit(1);
}

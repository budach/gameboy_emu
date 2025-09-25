#include <iostream>

#include "opcodes.h"
#include "gameboy.h"
#include "cpu.h"

uint8_t op_0x21_LD_HL_u16(Gameboy &gb)
{
    // set HL to the 16-bit value following the opcode
    gb.cpu.HL = gb.mmu.read16(gb.cpu.PC + 1);

    gb.cpu.PC += 3;
    return 12;
}

uint8_t op_0x31_LD_SP_u16(Gameboy &gb)
{
    // set SP to the 16-bit value following the opcode
    gb.cpu.SP = gb.mmu.read16(gb.cpu.PC + 1);

    gb.cpu.PC += 3;
    return 12;
}

uint8_t op_0x32_LD_HLm_A(Gameboy &gb)
{
    // store A into memory at address HL, then decrement HL
    gb.mmu.write8(gb.cpu.HL, gb.cpu.AF_bytes.A);
    gb.cpu.HL--;

    gb.cpu.PC += 1;
    return 8;
}

uint8_t op_0xAF_XOR_A_A(Gameboy &gb)
{
    gb.cpu.AF_bytes.A ^= gb.cpu.AF_bytes.A; // result is always 0
    gb.cpu.AF_bytes.F = CPU_FLAG_Z;         // Z flag set (bit 7), others cleared

    gb.cpu.PC += 1;
    return 4;
}

uint8_t op_0xCB_prefixed(Gameboy &gb)
{
    // get next byte to determine specific CB opcode
    uint8_t cb = gb.mmu.read8(gb.cpu.PC + 1);
    return gb.cb_opcodes[cb](gb);
}

uint8_t op_0xCB_0x7C_BIT_7_H(Gameboy &gb)
{
    // don't modify C flag, set H flag, clear N flag, set Z flag if bit 7 of H is 0, else clear
    gb.cpu.AF_bytes.F = (gb.cpu.AF_bytes.F & CPU_FLAG_C) | CPU_FLAG_H |
                        ((gb.cpu.HL_bytes.H & 0x80) == 0 ? CPU_FLAG_Z : 0);

    gb.cpu.PC += 2;
    return 8;
}

uint8_t op_0x20_JR_NZ_i8(Gameboy &gb)
{
    int8_t offset = static_cast<int8_t>(gb.mmu.read8(gb.cpu.PC + 1)); // read signed 8-bit offset

    // move to next instruction first, because offset is relative from there
    gb.cpu.PC += 2;

    if ((gb.cpu.AF_bytes.F & CPU_FLAG_Z) == 0) // Z flag not set
    {
        gb.cpu.PC += offset; // now apply relative jump
        return 12;
    }

    return 8;
}

uint8_t op_0x0E_LD_C_u8(Gameboy &gb)
{
    gb.cpu.BC_bytes.C = gb.mmu.read8(gb.cpu.PC + 1); // load 8-bit value into C register

    gb.cpu.PC += 2;
    return 8;
}

uint8_t op_0x3E_LD_A_u8(Gameboy &gb)
{
    gb.cpu.AF_bytes.A = gb.mmu.read8(gb.cpu.PC + 1); // load 8-bit value into A register

    gb.cpu.PC += 2;
    return 8;
}

uint8_t op_0xE2_LD_C_A(Gameboy &gb)
{
    gb.mmu.write8(0xFF00 + gb.cpu.BC_bytes.C, gb.cpu.AF_bytes.A); // write A to address (0xFF00 + C)

    gb.cpu.PC += 1;
    return 8;
}

uint8_t op_0x0C_INC_C(Gameboy &gb)
{
    gb.cpu.BC_bytes.C += 1; // increment C

    gb.cpu.AF_bytes.F &= CPU_FLAG_C;                                       // preserve C flag, clear others
    gb.cpu.AF_bytes.F |= ((gb.cpu.BC_bytes.C == 0) * CPU_FLAG_Z);          // Z flag if result is 0
    gb.cpu.AF_bytes.F |= (((gb.cpu.BC_bytes.C & 0x0F) == 0) * CPU_FLAG_H); // H flag if low nibble overflowed

    gb.cpu.PC += 1;
    return 4;
}

uint8_t op_0x77_LD_HL_A(Gameboy &gb)
{
    gb.mmu.write8(gb.cpu.HL, gb.cpu.AF_bytes.A); // write A to address HL

    gb.cpu.PC += 1;
    return 8;
}

uint8_t op_0xE0_LD_u8_A(Gameboy &gb)
{
    // write A to address (0xFF00 + u8)
    gb.mmu.write8(0xFF00 + gb.mmu.read8(gb.cpu.PC + 1), gb.cpu.AF_bytes.A);

    gb.cpu.PC += 2;
    return 12;
}

uint8_t op_0x11_LD_DE_u16(Gameboy &gb)
{
    gb.cpu.DE = gb.mmu.read16(gb.cpu.PC + 1); // set DE to the 16-bit value following the opcode

    gb.cpu.PC += 3;
    return 12;
}

uint8_t op_0x1A_LD_A_DE(Gameboy &gb)
{
    gb.cpu.AF_bytes.A = gb.mmu.read8(gb.cpu.DE); // load A from memory at address DE

    gb.cpu.PC += 1;
    return 8;
}

uint8_t op_0xCD_CALL_u16(Gameboy &gb)
{
    uint16_t addr = gb.mmu.read16(gb.cpu.PC + 1); // get 16-bit address to call

    // push address of next instruction (after CALL) onto stack
    // current CALL instruction has length 3 bytes (opcode + 16-bit address)
    gb.cpu.SP -= 2;
    gb.mmu.write16(gb.cpu.SP, gb.cpu.PC + 3);

    gb.cpu.PC = addr; // jump to called address
    return 24;
}

uint8_t op_0x4F_LD_C_A(Gameboy &gb)
{
    gb.cpu.BC_bytes.C = gb.cpu.AF_bytes.A; // copy A into C

    gb.cpu.PC += 1;
    return 4;
}

uint8_t op_0x06_LD_B_u8(Gameboy &gb)
{
    gb.cpu.BC_bytes.B = gb.mmu.read8(gb.cpu.PC + 1); // load 8-bit value into B register

    gb.cpu.PC += 2;
    return 8;
}

uint8_t op_0xC5_PUSH_BC(Gameboy &gb)
{
    gb.cpu.SP -= 2;                       // decrement stack pointer by 2
    gb.mmu.write16(gb.cpu.SP, gb.cpu.BC); // write BC to memory at address SP

    gb.cpu.PC += 1;
    return 16;
}

uint8_t op_0xCB_0x11_RL_C(Gameboy &gb)
{
    uint8_t old_c = gb.cpu.BC_bytes.C;
    uint8_t carry_in = (gb.cpu.AF_bytes.F & CPU_FLAG_C) ? 1 : 0;

    // rotate left through carry
    gb.cpu.BC_bytes.C = (old_c << 1) | carry_in;

    gb.cpu.AF_bytes.F = 0;                                        // clear all flags
    gb.cpu.AF_bytes.F |= ((gb.cpu.BC_bytes.C == 0) * CPU_FLAG_Z); // Z flag if result is 0
    gb.cpu.AF_bytes.F |= ((old_c & 0x80) >> 7) * CPU_FLAG_C;      // C flag if old bit 7 was set

    gb.cpu.PC += 2;
    return 8;
}

uint8_t op_0x17_RLA(Gameboy &gb)
{
    uint8_t old_a = gb.cpu.AF_bytes.A;
    uint8_t carry_in = (gb.cpu.AF_bytes.F & CPU_FLAG_C) ? 1 : 0;

    // rotate left through carry
    gb.cpu.AF_bytes.A = (old_a << 1) | carry_in;

    gb.cpu.AF_bytes.F = 0;                                   // clear all flags
    gb.cpu.AF_bytes.F |= ((old_a & 0x80) >> 7) * CPU_FLAG_C; // C flag if old bit 7 was set

    gb.cpu.PC += 1;
    return 4;
}

uint8_t op_0xC1_POP_BC(Gameboy &gb)
{
    gb.cpu.BC = gb.mmu.read16(gb.cpu.SP); // read 16-bit value from memory at address SP into BC
    gb.cpu.SP += 2;                       // increment stack pointer by 2

    gb.cpu.PC += 1;
    return 12;
}

uint8_t op_0x05_DEC_B(Gameboy &gb)
{
    gb.cpu.BC_bytes.B -= 1;

    gb.cpu.AF_bytes.F &= CPU_FLAG_C;                                          // preserve C flag, clear others
    gb.cpu.AF_bytes.F |= CPU_FLAG_N;                                          // set N flag (bit 6)
    gb.cpu.AF_bytes.F |= ((gb.cpu.BC_bytes.B == 0) * CPU_FLAG_Z);             // Z flag if result is 0
    gb.cpu.AF_bytes.F |= (((gb.cpu.BC_bytes.B & 0x0F) == 0x0F) * CPU_FLAG_H); // H flag if borrow from bit 4

    gb.cpu.PC += 1;
    return 4;
}

uint8_t op_0x22_LD_HLp_A(Gameboy &gb)
{
    gb.mmu.write8(gb.cpu.HL++, gb.cpu.AF_bytes.A); // store A into memory at address HL, then increment HL

    gb.cpu.PC += 1;
    return 8;
}

uint8_t op_0x23_INC_HL(Gameboy &gb)
{
    gb.cpu.HL += 1; // increment HL

    gb.cpu.PC += 1;
    return 8;
}

uint8_t op_0xC9_RET(Gameboy &gb)
{
    gb.cpu.PC = gb.mmu.read16(gb.cpu.SP); // pop return address from stack into PC
    gb.cpu.SP += 2;                       // increment stack pointer by 2

    return 16;
}

uint8_t op_0x13_INC_DE(Gameboy &gb)
{
    gb.cpu.DE += 1; // increment DE

    gb.cpu.PC += 1;
    return 8;
}

uint8_t op_0x7B_LD_A_E(Gameboy &gb)
{
    gb.cpu.AF_bytes.A = gb.cpu.DE_bytes.E; // copy E into A

    gb.cpu.PC += 1;
    return 4;
}

uint8_t op_0xFE_CP_A_u8(Gameboy &gb)
{
    uint8_t value = gb.mmu.read8(gb.cpu.PC + 1);
    uint8_t result = gb.cpu.AF_bytes.A - value;

    gb.cpu.AF_bytes.F = CPU_FLAG_N;                                                    // set N flag, clear others
    gb.cpu.AF_bytes.F |= ((result == 0) * CPU_FLAG_Z);                                 // Z flag if result is 0
    gb.cpu.AF_bytes.F |= (((gb.cpu.AF_bytes.A & 0x0F) < (value & 0x0F)) * CPU_FLAG_H); // H flag if borrow from bit 4
    gb.cpu.AF_bytes.F |= ((gb.cpu.AF_bytes.A < value) * CPU_FLAG_C);                   // C flag if borrow (A < value)

    gb.cpu.PC += 2;
    return 8;
}

uint8_t op_0xEA_LD_u16_A(Gameboy &gb)
{
    uint16_t addr = gb.mmu.read16(gb.cpu.PC + 1);
    gb.mmu.write8(addr, gb.cpu.AF_bytes.A); // write A to address

    gb.cpu.PC += 3;
    return 16;
}

uint8_t op_0x3D_DEC_A(Gameboy &gb)
{
    gb.cpu.AF_bytes.A -= 1;

    gb.cpu.AF_bytes.F &= CPU_FLAG_C;                                          // preserve C flag, clear others
    gb.cpu.AF_bytes.F |= CPU_FLAG_N;                                          // set N flag
    gb.cpu.AF_bytes.F |= ((gb.cpu.AF_bytes.A == 0) * CPU_FLAG_Z);             // Z flag if result is 0
    gb.cpu.AF_bytes.F |= (((gb.cpu.AF_bytes.A & 0x0F) == 0x0F) * CPU_FLAG_H); // H flag if borrow from bit 4

    gb.cpu.PC += 1;
    return 4;
}

uint8_t op_0x28_JR_Z_i8(Gameboy &gb)
{
    int8_t offset = static_cast<int8_t>(gb.mmu.read8(gb.cpu.PC + 1)); // read signed 8-bit offset

    gb.cpu.PC += 2; // move to next instruction first, offset is relative from there

    if (gb.cpu.AF_bytes.F & CPU_FLAG_Z) // if Z flag set
    {
        gb.cpu.PC += offset;
        return 12;
    }

    return 8;
}

uint8_t op_0x0D_DEC_C(Gameboy &gb)
{
    gb.cpu.BC_bytes.C -= 1;

    gb.cpu.AF_bytes.F &= CPU_FLAG_C;                                          // preserve C flag, clear others
    gb.cpu.AF_bytes.F |= CPU_FLAG_N;                                          // set N flag
    gb.cpu.AF_bytes.F |= ((gb.cpu.BC_bytes.C == 0) * CPU_FLAG_Z);             // Z flag if result is 0
    gb.cpu.AF_bytes.F |= (((gb.cpu.BC_bytes.C & 0x0F) == 0x0F) * CPU_FLAG_H); // H flag if borrow from bit 4

    gb.cpu.PC += 1;
    return 4;
}

uint8_t op_0x2E_LD_L_u8(Gameboy &gb)
{
    gb.cpu.HL_bytes.L = gb.mmu.read8(gb.cpu.PC + 1); // load 8-bit value into L register

    gb.cpu.PC += 2;
    return 8;
}

uint8_t op_0x18_JR_i8(Gameboy &gb)
{
    int8_t offset = static_cast<int8_t>(gb.mmu.read8(gb.cpu.PC + 1)); // read signed 8-bit offset

    gb.cpu.PC += 2; // move to next instruction first, offset is relative from there
    gb.cpu.PC += offset;

    return 12;
}

uint8_t op_0x67_LD_H_A(Gameboy &gb)
{
    gb.cpu.HL_bytes.H = gb.cpu.AF_bytes.A; // copy A into H

    gb.cpu.PC += 1;
    return 4;
}

uint8_t op_0x57_LD_D_A(Gameboy &gb)
{
    gb.cpu.DE_bytes.D = gb.cpu.AF_bytes.A; // copy A into D

    gb.cpu.PC += 1;
    return 4;
}

uint8_t op_0x04_INC_B(Gameboy &gb)
{
    gb.cpu.BC_bytes.B += 1; // increment B

    gb.cpu.AF_bytes.F &= CPU_FLAG_C;                                       // preserve C flag, clear others
    gb.cpu.AF_bytes.F |= ((gb.cpu.BC_bytes.B == 0) * CPU_FLAG_Z);          // Z flag if result is 0
    gb.cpu.AF_bytes.F |= (((gb.cpu.BC_bytes.B & 0x0F) == 0) * CPU_FLAG_H); // H flag if low nibble overflowed

    gb.cpu.PC += 1;
    return 4;
}

uint8_t op_0x1E_LD_E_u8(Gameboy &gb)
{
    gb.cpu.DE_bytes.E = gb.mmu.read8(gb.cpu.PC + 1); // load 8-bit value into E register

    gb.cpu.PC += 2;
    return 8;
}

uint8_t op_0xF0_LD_A_FF00_u8(Gameboy &gb)
{
    // load A from address (0xFF00 + u8)
    gb.cpu.AF_bytes.A = gb.mmu.read8(0xFF00 + gb.mmu.read8(gb.cpu.PC + 1));

    gb.cpu.PC += 2;
    return 12;
}

uint8_t op_unimplemented(Gameboy &gb)
{
    std::cerr << "Unimplemented opcode: 0x" << std::hex
              << (int)gb.mmu.read8(gb.cpu.PC) << " at PC: " << gb.cpu.PC << std::dec
              << std::endl;

    std::cerr << "Next byte in memory: 0x" << std::hex
              << (int)gb.mmu.read8(gb.cpu.PC + 1) << std::dec << std::endl;
    exit(1);

    return 0;
}

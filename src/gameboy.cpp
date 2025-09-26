#include <iostream>
#include <string>

#include "opcodes.h"
#include "gameboy.h"

Gameboy::Gameboy(const std::string &game_rom_filename) : ppu(mmu)
{
    // init memory

    mmu.load_game_rom(game_rom_filename);

    // init opcodes

    for (size_t i = 0; i < GB_NUM_OPCODES; i++)
    {
        opcodes[i] = op_unimplemented;
        cb_opcodes[i] = op_unimplemented;
    }

    opcodes[0x00] = op_0x00_NOP;
    opcodes[0x01] = op_0x01_LD_BC_u16;
    opcodes[0x04] = op_0x04_INC_B;
    opcodes[0x05] = op_0x05_DEC_B;
    opcodes[0x06] = op_0x06_LD_B_u8;
    opcodes[0x0B] = op_0x0B_DEC_BC;
    opcodes[0x0C] = op_0x0C_INC_C;
    opcodes[0x0D] = op_0x0D_DEC_C;
    opcodes[0x0E] = op_0x0E_LD_C_u8;
    opcodes[0x11] = op_0x11_LD_DE_u16;
    opcodes[0x12] = op_0x12_LD_DE_A;
    opcodes[0x13] = op_0x13_INC_DE;
    opcodes[0x15] = op_0x15_DEC_D;
    opcodes[0x16] = op_0x16_LD_D_u8;
    opcodes[0x17] = op_0x17_RLA;
    opcodes[0x18] = op_0x18_JR_i8;
    opcodes[0x19] = op_0x19_ADD_HL_DE;
    opcodes[0x1A] = op_0x1A_LD_A_DE;
    opcodes[0x1C] = op_0x1C_INC_E;
    opcodes[0x1D] = op_0x1D_DEC_E;
    opcodes[0x1E] = op_0x1E_LD_E_u8;
    opcodes[0x20] = op_0x20_JR_NZ_i8;
    opcodes[0x21] = op_0x21_LD_HL_u16;
    opcodes[0x22] = op_0x22_LD_HLp_A;
    opcodes[0x23] = op_0x23_INC_HL;
    opcodes[0x24] = op_0x24_INC_H;
    opcodes[0x28] = op_0x28_JR_Z_i8;
    opcodes[0x2A] = op_0x2A_LD_A_HLp;
    opcodes[0x2E] = op_0x2E_LD_L_u8;
    opcodes[0x2F] = op_0x2F_CPL;
    opcodes[0x31] = op_0x31_LD_SP_u16;
    opcodes[0x32] = op_0x32_LD_HLm_A;
    opcodes[0x36] = op_0x36_LD_HL_u8;
    opcodes[0x3D] = op_0x3D_DEC_A;
    opcodes[0x47] = op_0x47_LD_B_A;
    opcodes[0x4F] = op_0x4F_LD_C_A;
    opcodes[0x56] = op_0x56_LD_D_HL;
    opcodes[0x57] = op_0x57_LD_D_A;
    opcodes[0x5E] = op_0x5E_LD_E_HL;
    opcodes[0x5F] = op_0x5F_LD_E_A;
    opcodes[0x67] = op_0x67_LD_H_A;
    opcodes[0x77] = op_0x77_LD_HL_A;
    opcodes[0x78] = op_0x78_LD_A_B;
    opcodes[0x79] = op_0x79_LD_A_C;
    opcodes[0x7B] = op_0x7B_LD_A_E;
    opcodes[0x7C] = op_0x7C_LD_A_H;
    opcodes[0x7D] = op_0x7D_LD_A_L;
    opcodes[0x7E] = op_0x7E_LD_A_HL;
    opcodes[0x86] = op_0x86_ADD_A_HL;
    opcodes[0x87] = op_0x87_ADD_A_A;
    opcodes[0x90] = op_0x90_SUB_A_B;
    opcodes[0x3E] = op_0x3E_LD_A_u8;
    opcodes[0xA1] = op_0xA1_AND_A_C;
    opcodes[0xA7] = op_0xA7_AND_A_A;
    opcodes[0xA9] = op_0xA9_XOR_A_C;
    opcodes[0xAF] = op_0xAF_XOR_A_A;
    opcodes[0xB0] = op_0xB0_OR_A_B;
    opcodes[0xB1] = op_0xB1_OR_A_C;
    opcodes[0XBE] = op_0xBE_CP_A_HL;
    opcodes[0xC1] = op_0xC1_POP_BC;
    opcodes[0xC3] = op_0xC3_JP_u16;
    opcodes[0xC5] = op_0xC5_PUSH_BC;
    opcodes[0xC8] = op_0xC8_RET_Z;
    opcodes[0xC9] = op_0xC9_RET;
    opcodes[0xCA] = op_0xCA_JP_Z_u16;
    opcodes[0xCB] = op_0xCB_prefixed;
    opcodes[0xCD] = op_0xCD_CALL_u16;
    opcodes[0xD1] = op_0xD1_POP_DE;
    opcodes[0xD5] = op_0xD5_PUSH_DE;
    opcodes[0xE0] = op_0xE0_LD_u8_A;
    opcodes[0xE1] = op_0xE1_POP_HL;
    opcodes[0xE2] = op_0xE2_LD_C_A;
    opcodes[0xE5] = op_0xE5_PUSH_HL;
    opcodes[0xE6] = op_0xE6_AND_A_u8;
    opcodes[0xE9] = op_0xE9_JP_HL;
    opcodes[0xEA] = op_0xEA_LD_u16_A;
    opcodes[0xEF] = op_0xEF_RST_28h;
    opcodes[0xF0] = op_0xF0_LD_A_FF00_u8;
    opcodes[0xF1] = op_0xF1_POP_AF;
    opcodes[0xF3] = op_0xF3_DI;
    opcodes[0xF5] = op_0xF5_PUSH_AF;
    opcodes[0xFA] = op_0xFA_LD_A_u16;
    opcodes[0xFB] = op_0xFB_EI;
    opcodes[0xFE] = op_0xFE_CP_A_u8;

    cb_opcodes[0x11] = op_0xCB_0x11_RL_C;
    cb_opcodes[0x37] = op_0xCB_0x37_SWAP_A;
    cb_opcodes[0x7C] = op_0xCB_0x7C_BIT_7_H;
    cb_opcodes[0x87] = op_0xCB_0x87_RES_0_A;
}

uint8_t Gameboy::run_opcode()
{
    bool should_enable_IME = cpu.IME_scheduled;

    uint8_t opcode = mmu.read8(cpu.PC);
    uint8_t cycles = opcodes[opcode](*this);

    if (should_enable_IME)
    {
        cpu.IME = true;
        cpu.IME_scheduled = false;
    }

    return cycles;
}
#include <iostream>
#include <string>

#include "opcodes.h"
#include "gameboy.h"

Gameboy::Gameboy(const std::string &boot_rom_filename, const std::string &game_rom_filename)
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

    mmu.load_boot_rom(boot_rom_filename);
    mmu.load_game_rom(game_rom_filename);

    // init opcodes

    for (size_t i = 0; i < GB_NUM_OPCODES; i++)
    {
        opcodes[i] = op_unimplemented;
        cb_opcodes[i] = op_unimplemented;
    }

    opcodes[0x04] = op_0x04_INC_B;
    opcodes[0x05] = op_0x05_DEC_B;
    opcodes[0x06] = op_0x06_LD_B_u8;
    opcodes[0x0C] = op_0x0C_INC_C;
    opcodes[0x0D] = op_0x0D_DEC_C;
    opcodes[0x0E] = op_0x0E_LD_C_u8;
    opcodes[0x11] = op_0x11_LD_DE_u16;
    opcodes[0x13] = op_0x13_INC_DE;
    opcodes[0x17] = op_0x17_RLA;
    opcodes[0x18] = op_0x18_JR_i8;
    opcodes[0x1A] = op_0x1A_LD_A_DE;
    opcodes[0x1E] = op_0x1E_LD_E_u8;
    opcodes[0x20] = op_0x20_JR_NZ_i8;
    opcodes[0x21] = op_0x21_LD_HL_u16;
    opcodes[0x22] = op_0x22_LD_HLp_A;
    opcodes[0x23] = op_0x23_INC_HL;
    opcodes[0x28] = op_0x28_JR_Z_i8;
    opcodes[0x2E] = op_0x2E_LD_L_u8;
    opcodes[0x31] = op_0x31_LD_SP_u16;
    opcodes[0x32] = op_0x32_LD_HLm_A;
    opcodes[0x3D] = op_0x3D_DEC_A;
    opcodes[0x4F] = op_0x4F_LD_C_A;
    opcodes[0x57] = op_0x57_LD_D_A;
    opcodes[0x67] = op_0x67_LD_H_A;
    opcodes[0x77] = op_0x77_LD_HL_A;
    opcodes[0x7B] = op_0x7B_LD_A_E;
    opcodes[0x3E] = op_0x3E_LD_A_u8;
    opcodes[0xAF] = op_0xAF_XOR_A_A;
    opcodes[0xC1] = op_0xC1_POP_BC;
    opcodes[0xC5] = op_0xC5_PUSH_BC;
    opcodes[0xC9] = op_0xC9_RET;
    opcodes[0xCB] = op_0xCB_prefixed;
    opcodes[0xCD] = op_0xCD_CALL_u16;
    opcodes[0xE0] = op_0xE0_LD_u8_A;
    opcodes[0xE2] = op_0xE2_LD_C_A;
    opcodes[0xEA] = op_0xEA_LD_u16_A;
    opcodes[0xF0] = op_0xF0_LD_A_FF00_u8;
    opcodes[0xFE] = op_0xFE_CP_A_u8;

    cb_opcodes[0x11] = op_0xCB_0x11_RL_C;
    cb_opcodes[0x7C] = op_0xCB_0x7C_BIT_7_H;
}

uint8_t Gameboy::run_opcode()
{
    uint8_t opcode = mmu.read8(PC);

    if (opcode != 0xCB)
    {
        std::cout << "Executing opcode:\t0x" << std::hex << (int)opcode
                  << " at PC: 0x" << PC << std::dec << std::endl;
    }
    else
    {
        uint8_t cb_opcode = mmu.read8(PC + 1);
        std::cout << "Executing CB opcode:\t0xCB 0x" << std::hex << (int)cb_opcode
                  << " at PC: 0x" << PC << std::dec << std::endl;
    }

    return opcodes[opcode](*this);
}

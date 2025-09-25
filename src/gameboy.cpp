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

    opcodes[0x06] = op_0x06_LD_B_u8;
    opcodes[0x0C] = op_0x0C_INC_C;
    opcodes[0x0E] = op_0x0E_LD_C_u8;
    opcodes[0x11] = op_0x11_LD_DE_u16;
    opcodes[0x1A] = op_0x1A_LD_A_DE;
    opcodes[0x20] = op_0x20_JR_NZ_i8;
    opcodes[0x21] = op_0x21_LD_HL_u16;
    opcodes[0x31] = op_0x31_LD_SP_u16;
    opcodes[0x32] = op_0x32_LD_HLm_A;
    opcodes[0x4F] = op_0x4F_LD_C_A;
    opcodes[0X77] = op_0x77_LD_HL_A;
    opcodes[0x3E] = op_0x3E_LD_A_u8;
    opcodes[0xAF] = op_0xAF_XOR_A_A;
    opcodes[0xC5] = op_0xC5_PUSH_BC;
    opcodes[0xCB] = op_0xCB_prefixed;
    opcodes[0xCD] = op_0xCD_CALL_u16;
    opcodes[0xE0] = op_0xE0_LD_u8_A;
    opcodes[0xE2] = op_0xE2_LD_C_A;

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

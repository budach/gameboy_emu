#pragma once

#include <string>
#include <cstdint>

struct Gameboy;

struct Opcode
{
    std::string name;
    void (*execute)(Gameboy &gb);
    uint8_t length;
    uint8_t tcycles;
    uint8_t mcycles;
};

void op_0x21_LD_HL_u16(Gameboy &gb);
void op_0x31_LD_SP_u16(Gameboy &gb);
void op_0x32_LD_HLm_A(Gameboy &gb);
void op_0xAF_XOR_A_A(Gameboy &gb);
void op_unimplemented(Gameboy &gb);

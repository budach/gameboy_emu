#pragma once

#include <string>
#include <cstdint>

struct Gameboy;

struct Opcode
{
    std::string name;
    uint8_t (*execute)(Gameboy &gb);
    uint8_t length;
};

// opcode function declarations (definitions in opcodes.cpp)
// they all return the number of t-cycles taken to execute
uint8_t op_0x21_LD_HL_u16(Gameboy &gb);
uint8_t op_0x31_LD_SP_u16(Gameboy &gb);
uint8_t op_0x32_LD_HLm_A(Gameboy &gb);
uint8_t op_0xAF_XOR_A_A(Gameboy &gb);
uint8_t op_unimplemented(Gameboy &gb);
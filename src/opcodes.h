#pragma once

#include <cstdint>

struct Gameboy;

// opcode function declarations (definitions in opcodes.cpp)
// they all return the number of t-cycles taken to execute
// they also advance the PC internally as needed

uint8_t op_0x0C_INC_C(Gameboy &gb);
uint8_t op_0x3E_LD_A_u8(Gameboy &gb);
uint8_t op_0x0E_LD_C_u8(Gameboy &gb);
uint8_t op_0x21_LD_HL_u16(Gameboy &gb);
uint8_t op_0x31_LD_SP_u16(Gameboy &gb);
uint8_t op_0x32_LD_HLm_A(Gameboy &gb);
uint8_t op_0xAF_XOR_A_A(Gameboy &gb);
uint8_t op_unimplemented(Gameboy &gb);
uint8_t op_0xCB_prefixed(Gameboy &gb);
uint8_t op_0xCB_0x7C_BIT_7_H(Gameboy &gb);
uint8_t op_0xCB_0x11_RL_C(Gameboy &gb);
uint8_t op_0x20_JR_NZ_i8(Gameboy &gb);
uint8_t op_0xE2_LD_C_A(Gameboy &gb);
uint8_t op_0x77_LD_HL_A(Gameboy &gb);
uint8_t op_0xE0_LD_u8_A(Gameboy &gb);
uint8_t op_0x11_LD_DE_u16(Gameboy &gb);
uint8_t op_0x1A_LD_A_DE(Gameboy &gb);
uint8_t op_0xCD_CALL_u16(Gameboy &gb);
uint8_t op_0x4F_LD_C_A(Gameboy &gb);
uint8_t op_0x06_LD_B_u8(Gameboy &gb);
uint8_t op_0xC5_PUSH_BC(Gameboy &gb);
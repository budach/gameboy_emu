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
uint8_t op_0x17_RLA(Gameboy &gb);
uint8_t op_0xC1_POP_BC(Gameboy &gb);
uint8_t op_0x05_DEC_B(Gameboy &gb);
uint8_t op_0x22_LD_HLp_A(Gameboy &gb);
uint8_t op_0x23_INC_HL(Gameboy &gb);
uint8_t op_0xC9_RET(Gameboy &gb);
uint8_t op_0x13_INC_DE(Gameboy &gb);
uint8_t op_0x7B_LD_A_E(Gameboy &gb);
uint8_t op_0xFE_CP_A_u8(Gameboy &gb);
uint8_t op_0xEA_LD_u16_A(Gameboy &gb);
uint8_t op_0x3D_DEC_A(Gameboy &gb);
uint8_t op_0x28_JR_Z_i8(Gameboy &gb);
uint8_t op_0x0D_DEC_C(Gameboy &gb);
uint8_t op_0x2E_LD_L_u8(Gameboy &gb);
uint8_t op_0x18_JR_i8(Gameboy &gb);
uint8_t op_0x67_LD_H_A(Gameboy &gb);
uint8_t op_0x57_LD_D_A(Gameboy &gb);
uint8_t op_0x04_INC_B(Gameboy &gb);
uint8_t op_0x1E_LD_E_u8(Gameboy &gb);
uint8_t op_0xF0_LD_A_FF00_u8(Gameboy &gb);
uint8_t op_0x1D_DEC_E(Gameboy &gb);
uint8_t op_0x24_INC_H(Gameboy &gb);
uint8_t op_0x7C_LD_A_H(Gameboy &gb);
uint8_t op_0x90_SUB_A_B(Gameboy &gb);
uint8_t op_0x15_DEC_D(Gameboy &gb);
uint8_t op_0x16_LD_D_u8(Gameboy &gb);
uint8_t op_0xBE_CP_A_HL(Gameboy &gb);
uint8_t op_0x7D_LD_A_L(Gameboy &gb);
uint8_t op_0x78_LD_A_B(Gameboy &gb);
uint8_t op_0x86_ADD_A_HL(Gameboy &gb);
uint8_t op_0x00_NOP(Gameboy &gb);
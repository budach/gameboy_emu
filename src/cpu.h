#pragma once

#include <cstdint>

// flag bit masks for the F register
constexpr uint8_t CPU_FLAG_Z = 1 << 7; // zero flag
constexpr uint8_t CPU_FLAG_N = 1 << 6; // subtract flag
constexpr uint8_t CPU_FLAG_H = 1 << 5; // half Carry flag
constexpr uint8_t CPU_FLAG_C = 1 << 4; // carry flag

struct CPU
{
    // registers
    union
    {
        uint16_t AF;
        struct
        {
            uint8_t F; // low
            uint8_t A; // high
        } AF_bytes;
    };

    union
    {
        uint16_t BC;
        struct
        {
            uint8_t C; // low
            uint8_t B; // high
        } BC_bytes;
    };

    union
    {
        uint16_t DE;
        struct
        {
            uint8_t E; // low
            uint8_t D; // high
        } DE_bytes;
    };

    union
    {
        uint16_t HL;
        struct
        {
            uint8_t L; // low
            uint8_t H; // high
        } HL_bytes;
    };

    uint16_t SP, PC; // stack pointer and program counter

    CPU();
};
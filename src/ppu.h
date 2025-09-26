#pragma once

#include <cstdint>

#include "mmu.h"

constexpr int PPU_MODE_HBLANK = 0;
constexpr int PPU_MODE_VBLANK = 1;
constexpr int PPU_MODE_OAM = 2;
constexpr int PPU_MODE_DRAWING = 3;

// pixel processing unit

struct PPU
{
    MMU &mmu;            // reference to MMU for memory access
    int scanline_cycles; // cycles spent on current scanline

    void step(int cycles); // advance PPU state by given CPU cycles
    void check_lyc();      // check LYC=LY coincidence and trigger interrupt if needed

    PPU(MMU &mmu_ref) : mmu(mmu_ref), scanline_cycles(0) {} // constructor
};

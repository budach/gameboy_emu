#include "gameboy.h"

int main()
{
    Gameboy gb("test_roms/game.gb");

    uint64_t cycles = 0;

    while (true)
    {
        uint8_t cycles_this_step = gb.run_opcode();
        cycles += cycles_this_step;
        gb.ppu.step(cycles_this_step);
    }

    return 0;
}

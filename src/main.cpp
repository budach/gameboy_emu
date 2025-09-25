#include "gameboy.h"

int main()
{
    Gameboy gb("test_roms/dmg_boot.bin", "test_roms/game.gb");

    uint64_t cycles = 0;

    while (true)
    {
        cycles += gb.run_opcode();
        // TODO handle timers, interrupts, etc. based on cycles
    }

    return 0;
}

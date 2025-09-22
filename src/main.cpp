#include "gameboy.h"

int main()
{
    Gameboy gb("test_roms/dmg_boot.gb");
    while (true)
    {
        gb.run_cycle();
    }
    return 0;
}

#include "cpu.h"

CPU::CPU()
{
    // state after boot rom execution
    AF = 0x01B0;
    BC = 0x0013;
    DE = 0x00D8;
    HL = 0x014D;
    SP = 0xFFFE;
    PC = 0x0100;

    IME = false;
    IME_scheduled = false;
}

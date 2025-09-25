#include "cpu.h"

CPU::CPU()
{
    // init registers

    AF = 0;
    BC = 0;
    DE = 0;
    HL = 0;

    // init stack pointer and program counter

    SP = 0;
    PC = 0;
}

#include "ppu.h"

void PPU::step(int cycles)
{
    // LCD is off, reset state
    if (!(mmu.read8(0xFF40) & 0x80))
    {
        scanline_cycles = 0;
        mmu.write8(0xFF44, 0);                                             // LY = 0
        mmu.write8(0xFF41, (mmu.read8(0xFF41) & ~0x03) | PPU_MODE_HBLANK); // mode = HBlank
        check_lyc();
        return;
    }

    scanline_cycles += cycles;

    switch (mmu.read8(0xFF41) & 0x03) // current LCD mode
    {

    case PPU_MODE_OAM:

        if (scanline_cycles >= 80)
        {
            scanline_cycles -= 80;
            mmu.write8(0xFF41, (mmu.read8(0xFF41) & ~0x03) | PPU_MODE_DRAWING); // switch mode
        }

        break;

    case PPU_MODE_DRAWING:

        if (scanline_cycles >= 172)
        {
            scanline_cycles -= 172;
            mmu.write8(0xFF41, (mmu.read8(0xFF41) & ~0x03) | PPU_MODE_HBLANK); // switch mode
            // TODO render scanline to buffer here
        }

        break;

    case PPU_MODE_HBLANK:

        if (scanline_cycles >= 204)
        {
            scanline_cycles -= 204;
            uint8_t LY = mmu.read8(0xFF44) + 1; // current scanline
            mmu.write8(0xFF44, LY);
            check_lyc();

            if (LY == 144)
            {
                mmu.write8(0xFF41, (mmu.read8(0xFF41) & ~0x03) | PPU_MODE_VBLANK); // switch mode
                // TODO draw frame here?
                // TODO request VBlank interrupt here?
            }
            else
            {
                mmu.write8(0xFF41, (mmu.read8(0xFF41) & ~0x03) | PPU_MODE_OAM); // switch mode
            }
        }

        break;

    case PPU_MODE_VBLANK:

        if (scanline_cycles >= 456)
        {
            scanline_cycles -= 456;
            uint8_t LY = mmu.read8(0xFF44) + 1; // current scanline
            mmu.write8(0xFF44, LY);
            check_lyc();

            if (LY > 153)
            {
                // start new frame
                mmu.write8(0xFF44, 0);                                          // reset LY to 0
                mmu.write8(0xFF41, (mmu.read8(0xFF41) & ~0x03) | PPU_MODE_OAM); // switch mode
                check_lyc();
            }
        }

        break;
    }
}

void PPU::check_lyc()
{
    uint8_t LY = mmu.read8(0xFF44);  // Current scanline
    uint8_t LYC = mmu.read8(0xFF45); // LYC register

    uint8_t stat = mmu.read8(0xFF41); // LCD STAT register
    bool prevCoinc = stat & 0x04;     // Previous coincidence flag

    if (LY == LYC)
    {
        // Set coincidence flag
        stat |= 0x04;

        // If flag just became set and LYC interrupt is enabled (bit 6)
        if (!prevCoinc && (stat & 0x40))
        {
            // TODO request LCD STAT interrupt
        }
    }
    else
    {
        // Clear coincidence flag
        stat &= ~0x04;
    }

    mmu.write8(0xFF41, stat);
}
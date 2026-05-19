#include "PauseMenuPage.h"

#include "common.h"

#include "DiskChange.h"
#include "Map.h"
#include "PauseMenu.h"
#include "command.h"
#include "main.h"

#include "xgl/render.h"

extern u16 D_00490DBA[8];
extern char D_004C0300[];
extern char D_004D9EF8[];
extern u8 actor[];

void GameResourceDump(s32 arg);
void xglFontDebugPrintf(s32 x, s32 y, const char* fmt, ...);

// Matched. The `new_var` setup and the dead-code `if` in the loop are
// required to coax ee-gcc2.96 into the target's s0/s1 register choice
// (s0=i, s1=a). Without them GCC swaps the allocation. !D_004D9EF8 is
// always 0 (the symbol's address is non-zero at link time), so the
// inner block is provably unreachable but the compiler can't prove
// that pre-link.
void PauseMenuPage0()
{
    s32 i;
    u8* a;
    s32 y;
    u8* base;
    int new_var;

    if (D_00490DBA[0] & 0x40)
    {
        GameResourceDump(0);
    }

    xglFontDebugPrintf(0x88, 0x34, D_004C0300, WorkEnd);

    base = actor;
    a = base + 0x80;
    y = 0x40;
    new_var = !D_004D9EF8;
    for (i = 15; i >= 0; --i)
    {
        xglFontDebugPrintf(8, y, D_004D9EF8, *(s16*)(a + 6));
        if ((new_var && (!D_004D9EF8)) && (!D_004D9EF8))
        {
        }
        a += 0xA70;
        y += 8;
    }

    GameResourceDump(1);
}

INCLUDE_ASM("asm/nonmatchings/game/PauseMenuPage", PauseMenuPage2);

INCLUDE_ASM("asm/nonmatchings/game/PauseMenuPage", PauseMenuPage3);

INCLUDE_ASM("asm/nonmatchings/game/PauseMenuPage", GameDebugMenu);

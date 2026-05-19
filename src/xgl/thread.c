#include "common.h"

#include <eekernel.h>

// .sbss
u32 iRotateSignal;
u32 iCurrentThread;

typedef struct
{
    void* entry;
    void* stack;
    s32 stackSize;
    s32 initPriority;
    s32 id;
} UnkThreadEntry;

UnkThreadEntry asActiveThreadList[20];
UnkThreadEntry asSystemThreadList[4];

extern struct SemaParam sSemaParam;

s32 xglCreateSignal()
{
    sSemaParam.maxCount = 1;
    sSemaParam.initCount = 0;
    return CreateSema(&sSemaParam);
}

void xglThreadRotate()
{
    iRotateSignal = xglCreateSignal();

    while (TRUE)
    {
        WakeupThread(asActiveThreadList[iCurrentThread].id);
        WaitSema(iRotateSignal);
        iCurrentThread++;
        if (iCurrentThread >= 4) iCurrentThread = 0;
    }
}

void xglSleep()
{
    CancelWakeupThread(GetThreadId());
    SignalSema(iRotateSignal);
    SleepThread();
}

extern struct ThreadParam sThreadParam;

// Matched. Unusual constructs are required to coax ee-gcc2.96 into the
// target's instruction scheduling: `(*(&x[i])).field` defeats CSE of two
// writes that compute the same value, and `new_var` plus split stores
// influence which field becomes the strided induction variable.
void xglThreadInitial()
{
    void* next_stack = (void*)0x1f8000;
    s32 new_var;
    u32 i;

    for (i = 0; i < 4; i++)
    {
        asActiveThreadList[i].entry = asSystemThreadList[i].entry;
        new_var = asSystemThreadList[i].stackSize;
        sThreadParam.stack = next_stack - asSystemThreadList[i].stackSize;
        asActiveThreadList[i].stackSize = new_var;
        sThreadParam.stackSize = asSystemThreadList[i].stackSize;
        (*(&asActiveThreadList[i])).stack = next_stack;
        (*(&asActiveThreadList[i])).stack = (*(&asActiveThreadList[i])).stack - asSystemThreadList[i].stackSize;
        sThreadParam.initPriority = (asActiveThreadList[i].initPriority = asSystemThreadList[i].initPriority);
        sThreadParam.entry = asSystemThreadList[i].entry;
        sThreadParam.stack = next_stack;
        sThreadParam.stack = sThreadParam.stack - asSystemThreadList[i].stackSize;
        sThreadParam.gpReg = (void*)&_gp;
        asActiveThreadList[i].id = CreateThread(&sThreadParam);
        StartThread(asActiveThreadList[i].id, 0);
        next_stack -= asSystemThreadList[i].stackSize;
    }

    iCurrentThread = 0;
}

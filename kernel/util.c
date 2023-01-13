#include "../include/kernel/util.h"

void micro_delay()
{
    asm volatile("nop");
}
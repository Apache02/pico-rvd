#include "commands.h"

#ifndef DUMP_DEFAULT_ADDRESS
#define DUMP_DEFAULT_ADDRESS     (0x08000000)
#endif


static inline bool check(Console &c) {
    if (!c.rvd) {
        printf_r("rvd is null\n");
        return false;
    }

    return true;
}


void command_dump(Console &c) {
    if (!check(c)) return;

    auto addr = c.packet.take_int().ok_or(DUMP_DEFAULT_ADDRESS);
    printf("addr 0x%08x\n", addr);

    if (addr & 3) {
        printf("dump - bad addr 0x%08x\n", addr);
    } else {
        uint32_t buf[24 * 8];
        c.rvd->get_block_aligned(addr, buf, 24 * 8 * 4);
        for (int y = 0; y < 24; y++) {
            for (int x = 0; x < 8; x++) {
                printf("0x%08x ", buf[x + 8 * y]);
            }
            printf("\n");
        }
    }
}

void command_reset(Console &c) {
    if (!check(c)) return;

    if (c.rvd->reset()) {
        printf_g("Reset OK\n");
    } else {
        printf_r("Reset failed\n");
    }
}

void command_halt(Console &c) {
    if (!check(c)) return;

    if (c.rvd->halt()) {
        printf_g("Halted at DPC = 0x%08x\n", c.rvd->get_dpc());
    } else {
        printf_r("Halt failed\n");
    }
}

void command_resume(Console &c) {
    if (!check(c)) return;

    if (c.rvd->resume()) {
        printf_g("Resume OK\n");
    } else {
        printf_r("Resume failed\n");
    }
}

void command_step(Console &c) {
    if (!check(c)) return;

    int count = c.packet.take_int().ok_or(1);
    if (count < 1) {
        printf_r("Invalid step count\n");
        return;
    }

    for (int i=0; i<count; i++) {
        if (c.rvd->step()) {
            printf_g("%d. Stepped to DPC = 0x%08x\n", i, c.rvd->get_dpc());
        } else {
            printf_r("Step failed\n");
            break;
        }
    }

}

void command_status(Console &c) {
    if (!check(c)) return;

    c.rvd->dump();
}

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
        return;
    }

    uint32_t buf[24 * 8];
    c.rvd->get_block_aligned(addr, buf, 24 * 8 * 4);
    for (int y = 0; y < 24; y++) {
        for (int x = 0; x < 8; x++) {
            printf("0x%08x ", buf[x + 8 * y]);
        }
        printf("\n");
    }
}

void command_dump2(Console &c) {
    if (!check(c)) return;

    auto addr = c.packet.take_int().ok_or(DUMP_DEFAULT_ADDRESS);
    printf("addr 0x%08x\n", addr);

    if (addr & 3) {
        printf("dump - bad addr 0x%08x\n", addr);
        return;
    }

    const unsigned int per_line = 8;

    printf("         | 00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f 10 11 12 13 14 15 16 17 18 19 1a 1b 1c 1d 1e 1f \n");
    printf("---------|-------------------------------------------------------------------------------------------------\n");

    uint32_t buf[24 * per_line];
    c.rvd->get_block_aligned(addr, buf, sizeof(buf));
    for (int y = 0; y < 24; y++) {
        printf("%08x | ", addr + y * per_line * sizeof(buf[0]));
        for (int x = 0; x < per_line; x++) {
            uint32_t n = buf[x + per_line * y];
            printf(
                    "%02x %02x %02x %02x ",
                    ((n >> 0) & 0xFF),
                    ((n >> 8) & 0xFF),
                    ((n >> 16) & 0xFF),
                    ((n >> 24) & 0xFF)
            );
        }
        printf("\n");
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

    for (int i = 0; i < count; i++) {
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

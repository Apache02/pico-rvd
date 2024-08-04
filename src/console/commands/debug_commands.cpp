#include "commands.h"

#ifndef DUMP_DEFAULT_ADDRESS
#define DUMP_DEFAULT_ADDRESS     (0x08000000)
#endif


static inline bool check(Application &app) {
    if (!app.rvd) {
        printf_r("rvd is null\n");
        return false;
    }

    return true;
}


void command_dump(Application &app, Console &c) {
    if (!check(app)) return;

    auto addr = c.packet.take_int().ok_or(DUMP_DEFAULT_ADDRESS);
    printf("addr 0x%08x\n", addr);

    if (addr & 3) {
        printf("dump - bad addr 0x%08x\n", addr);
        return;
    }

    uint32_t buf[24 * 8];
    app.rvd->get_block_aligned(addr, buf, 24 * 8 * 4);
    for (int y = 0; y < 24; y++) {
        for (int x = 0; x < 8; x++) {
            printf("0x%08x ", buf[x + 8 * y]);
        }
        printf("\n");
    }
}

void command_dump2(Application &app, Console &c) {
    if (!check(app)) return;

    auto addr = c.packet.take_int().ok_or(DUMP_DEFAULT_ADDRESS);
    printf("addr 0x%08x\n", addr);

    if (addr & 3) {
        printf("dump - bad addr 0x%08x\n", addr);
        return;
    }

    const unsigned int per_line = 8;

    // ------------------------
    // header
    printf("         | ");
    for (int x = 0; x < per_line; x++) {
        printf("%02X %02X %02X %02X | ", (x * 4 + 0), (x * 4 + 1), (x * 4 + 2), (x * 4 + 3));
    }
    printf("\n---------|-");
    for (int x = 0; x < per_line; x++) {
        printf("--------------");
    }
    printf("\n");

    // ------------------------
    // body
    uint32_t buf[24 * per_line];
    app.rvd->get_block_aligned(addr, buf, sizeof(buf));
    for (int y = 0; y < 24; y++) {
        printf("%08x | ", addr + y * per_line * sizeof(buf[0]));
        for (int x = 0; x < per_line; x++) {
            uint32_t n = buf[x + per_line * y];
            printf(
                    "%02X %02X %02X %02X | ",
                    ((n >> 0) & 0xFF),
                    ((n >> 8) & 0xFF),
                    ((n >> 16) & 0xFF),
                    ((n >> 24) & 0xFF)
            );
        }
        printf("\n");
    }
}

void command_reset(Application &app, Console &c) {
    if (!check(app)) return;

    if (app.rvd->reset()) {
        printf_g("Reset OK\n");
    } else {
        printf_r("Reset failed\n");
    }
}

void command_halt(Application &app, Console &c) {
    if (!check(app)) return;

    if (app.rvd->halt()) {
        printf_g("Halted at DPC = 0x%08x\n", app.rvd->get_dpc());
    } else {
        printf_r("Halt failed\n");
    }
}

void command_resume(Application &app, Console &c) {
    if (!check(app)) return;

    if (app.rvd->resume()) {
        printf_g("Resume OK\n");
    } else {
        printf_r("Resume failed\n");
    }
}

void command_step(Application &app, Console &c) {
    if (!check(app)) return;

    int count = c.packet.take_int().ok_or(1);
    if (count < 1) {
        printf_r("Invalid step count\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        if (app.rvd->step()) {
            printf_g("%d. Stepped to DPC = 0x%08x\n", i, app.rvd->get_dpc());
        } else {
            printf_r("Step failed\n");
            break;
        }
    }

}

void command_status(Application &app, Console &c) {
    if (!check(app)) return;

    app.rvd->dump();
}

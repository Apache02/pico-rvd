#include "commands.h"
#include "pico/time.h"

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

void command_init_swio(Application &app, Console &c) {
    if (!check(app)) return;

    app.swio->reset();
}

void command_part_id(Application &app, Console &c) {
    if (!check(app)) return;

    uint32_t part_id = app.swio->get_partid();
    printf_g("DM_PARTID = 0x%08X\n", part_id);
}

// When swio pin stay HIGH or LOW all reads returns only 1 or 0
// part_id should be not 0xffffffff of 0x00000000, so we can use it to identify when swio is actually in debug mode
static bool check_part_id(Application &app) {
    uint32_t part_id = app.swio->get_partid();
    if (part_id == 0 || part_id == 0xffffffff) {
        return false;
    }

    // In theory it's possible to found moment when pin switching and part_id looks like 0xFFFF0000
    // so double check part id for sure
    return part_id == app.swio->get_partid();
}

void command_halt_on_reset(Application &app, Console &c) {
    if (!check(app)) return;

    printf("Trying enter debug mode in normal way... ");
    app.rvd->halt();
    if (!check_part_id(app)) {
        app.swio->reset();
        app.rvd->halt();
    }

    if (check_part_id(app)) {
        printf_g("success\n");
        return;
    }

    printf_r("fail\n");

    printf_w("\nPlease toggle chips VCC pin in next 10 seconds...\n");
    auto start_time = get_absolute_time();
    static const char progress_array[] = {'-', '\\', '|', '/'};
    printf(" ");

    for (int i = 0;;) {
        printf("\b%c", progress_array[i]);
        if (++i >= count_of(progress_array)) i = 0;

        app.swio->reset();
        app.rvd->halt();

        if (check_part_id(app)) {
            sleep_us(1000);
            if (check_part_id(app)) {
                printf_g("\bSuccessfully halted\n");
                break;
            }
        }

        if (absolute_time_diff_us(start_time, get_absolute_time()) > 10'000'000) {
            printf_r("\bHalt failed\n");
            break;
        }
    }
}

void command_chip_id(Application &app, Console &c) {
    if (!check(app)) return;

    static const struct {
        uint32_t part_id;
        const char *name;
    } part_id_map[] = {
            {0x00300500, "CH32V003F4P6"},
            {0x00310500, "CH32V003F4U6"},
            {0x00320500, "CH32V003A4M6"},
            {0x00330500, "CH32V003J4M6"},
    };

    struct {
        uint32_t unk_01;
        uint32_t part_id;
        uint32_t unk_02;
        uint32_t unk_03;
        uint32_t unk_04;
        uint32_t unk_05;
        uint32_t unk_06;
        uint32_t unk_07;
        uint16_t R16_ESIG_FLACAP;
        uint16_t pad_01;
        uint32_t pad_02;
        uint32_t R32_ESIG_UNIID1;
        uint32_t R32_ESIG_UNIID2;
        uint32_t R32_ESIG_UNIID3;
    } buf;

    app.rvd->get_block_aligned(0x1FFFF7C0, &buf, sizeof(buf));

    const char *part_id_name = "Unknown";
    auto part_id_fact = buf.part_id & 0xFFFFFF0F;
    for (auto[part_id_cmp, name]: part_id_map) {
        if (part_id_fact == part_id_cmp) {
            part_id_name = name;
        }
    }

    printf("Chip: " "\u001b[%sm" "%s" "\u001b[0m" "\n", "1;37", part_id_name);
    printf("Part ID: %08lX\n", buf.part_id);
    printf("Flash Capacity: %d Kb\n", buf.R16_ESIG_FLACAP);
    printf("Unique ID: %08lX%08lX%08lX\n", buf.R32_ESIG_UNIID1, buf.R32_ESIG_UNIID2, buf.R32_ESIG_UNIID3);
}

#include "shell_task.h"
#include "shell_commands/commands.h"
#include "Application.h"
#include <FreeRTOS.h>
#include <task.h>
#include "tusb.h"
#include "usb_itf.h"

static void help(Console &console);

static const Console::Handler handlers[] = {
        {"help",          help},
        {"pico_clocks",   command_clocks},
        {"init_swio",     command_init_swio},
        {"reset",         command_reset},
        {"halt",          command_halt},
        {"resume",        command_resume},
        {"step",          command_step},
        {"dump",          command_dump},
        {"dump2",         command_dump2},
        {"status",        command_status},
        {"part_id",       command_part_id},
        {"chip_id",       command_chip_id},
        {"wipe_chip",     command_wipe},
        {"halt_on_reset", command_halt_on_reset},
        {NULL, NULL},
};

void help(Console &console) {
    printf("Commands:\n");
    for (int i = 0;; i++) {
        if (!handlers[i].name || !handlers[i].handler) {
            break;
        }

        printf("  %s\n", handlers[i].name);
    }
}

static bool is_connected() {
    return tud_cdc_n_connected(ITF_CONSOLE);
}

static void wait_usb() {
    vTaskDelay(pdMS_TO_TICKS(500));
    while (!is_connected()) {
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    vTaskDelay(pdMS_TO_TICKS(100));
}

void vTaskShell(__unused void *pvParams) {
    printf(COLOR_GREEN("// Starting shell") "\n");
    auto *console = new Console(handlers);

    vTaskDelay(pdMS_TO_TICKS(500));

    for (;;) {
        wait_usb();

        console->reset();
        console->start();

        while (is_connected()) {
            char rx;
            if (tud_cdc_n_read(ITF_CONSOLE, &rx, sizeof(rx)) > 0) {
                console->update(rx);
            }

            vTaskDelay(1);
        }
    }
}

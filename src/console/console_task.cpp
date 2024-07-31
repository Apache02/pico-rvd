#include "console_task.h"
#include "Console.h"
#include <FreeRTOS.h>
#include <task.h>
#include "tusb.h"

void vTaskConsole(__unused void *pvParams) {
    Console *console = new Console();

    vTaskDelay(pdMS_TO_TICKS(1000));

    console->reset();
    console->start();

    char rx[1];

    for (;;) {
        size_t count = tud_cdc_n_read(0, rx, sizeof(rx));
        console->update(count > 0, rx[0]);

        vTaskDelay(1);
    }
}

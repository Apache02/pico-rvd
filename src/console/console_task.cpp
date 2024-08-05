#include "console_task.h"
#include "Application.h"
#include <FreeRTOS.h>
#include <task.h>
#include "tusb.h"
#include "usb_itf.h"


void vTaskConsole(__unused void *pvParams) {
    vTaskDelay(pdMS_TO_TICKS(500));

    gApp->console->reset();
    gApp->console->start();

    char rx[1];

    for (;;) {
        size_t count = tud_cdc_n_read(ITF_CONSOLE, rx, sizeof(rx));
        gApp->console->update(count > 0, rx[0]);

        vTaskDelay(1);
    }
}

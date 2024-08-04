#include "console_task.h"
#include "Console.h"
#include <FreeRTOS.h>
#include <task.h>
#include "tusb.h"
#include "RVDebug.h"
#include "PicoSWIO.h"

void vTaskConsole(__unused void *pvParams) {
    vTaskDelay(pdMS_TO_TICKS(1000));

    printf_g("// Starting PicoSWIO\n");
    PicoSWIO *swio = new PicoSWIO();
    swio->reset(5);

    printf_g("// Starting RVDebug\n");
    RVDebug *rvd = new RVDebug(swio, 16);
    rvd->init();

    printf_g("// Starting Console\n");
    Console *console = new Console(rvd);

    console->reset();
    console->start();

    char rx[1];

    for (;;) {
        size_t count = tud_cdc_n_read(0, rx, sizeof(rx));
        console->update(count > 0, rx[0]);

        vTaskDelay(1);
    }

    delete console;
}

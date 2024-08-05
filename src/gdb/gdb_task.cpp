#include "gdb_task.h"
#include "Application.h"
#include <FreeRTOS.h>
#include <task.h>
#include "tusb.h"
#include "usb_itf.h"


void vTaskGdb(__unused void *pvParams) {
    vTaskDelay(pdMS_TO_TICKS(500));

    for (;;) {
        bool connected = tud_cdc_n_connected(ITF_GDB);
        char rx = 0;
        bool rx_enable = tud_cdc_n_available(ITF_GDB); // this "available" check is required for some reason
        char tx = 0;
        bool tx_enable = 0;

        if (rx_enable) {
            tud_cdc_n_read(ITF_GDB, &rx, 1);
        }

        gApp->gdb->update(connected, rx_enable, rx, tx_enable, tx);

        if (tx_enable) {
            tud_cdc_n_write(ITF_GDB, &tx, 1);
            tud_cdc_n_write_flush(ITF_GDB);
        }
    }
}

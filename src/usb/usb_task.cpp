#include <FreeRTOS.h>
#include <task.h>
#include "pico/stdlib.h"
#include "pico/bootrom.h"
#include "tusb.h"

void vTaskUsb(__unused void *pvParams) {
    tusb_init();

    cdc_line_coding_t lc;

    for (;;) {
        tud_cdc_n_get_line_coding(0, &lc);

        if (lc.bit_rate == PICO_STDIO_USB_RESET_MAGIC_BAUD_RATE) {
            reset_usb_boot(0, PICO_STDIO_USB_RESET_BOOTSEL_INTERFACE_DISABLE_MASK);
        }

        tud_task();
        vTaskDelay(1);
    }
}

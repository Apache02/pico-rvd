#include <FreeRTOS.h>
#include <task.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "tusb.h"

#include "usb/usb_task.h"
#include "usb/tusb_config.h"
#include "usb/usb_itf.h"
#include "console/console_task.h"
#include "gdb/gdb_task.h"

#include "Application.h"


#ifdef PICO_DEFAULT_LED_PIN

void vTaskStatusLed(__unused void *pvParams) {
    bi_decl(bi_1pin_with_name(PICO_DEFAULT_LED_PIN, "On-board LED"));

    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_LED_PIN, 0);

    for (;;) {
        gpio_put(PICO_DEFAULT_LED_PIN, tud_cdc_n_connected(ITF_GDB));
        vTaskDelay(10);
    }
}

#endif

void board_init() {
    usbd_serial_init();
    set_sys_clock_khz(configCPU_CLOCK_HZ / 1000, false);
    stdio_init_all();
}

void application_init() {
    gApp = new Application();
    gApp->init();
}

int main() {
    bi_decl(bi_program_description("ch32v003 debugger binary."));

    board_init();
    application_init();

    xTaskCreate(
            vTaskUsb,
            "usb",
            configMINIMAL_STACK_SIZE,
            NULL,
            3,
            NULL
    );

    xTaskCreate(
            vTaskConsole,
            "console",
            configMINIMAL_STACK_SIZE * 4,
            NULL,
            2,
            NULL
    );

    xTaskCreate(
            vTaskGdb,
            "gdb",
            configMINIMAL_STACK_SIZE * 4,
            NULL,
            2,
            NULL
    );

#ifdef PICO_DEFAULT_LED_PIN
    xTaskCreate(
            vTaskStatusLed,
            "status_led",
            configMINIMAL_STACK_SIZE,
            NULL,
            1,
            NULL
    );
#endif

    vTaskStartScheduler();

    return 0;
}

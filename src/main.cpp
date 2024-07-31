#include <FreeRTOS.h>
#include <task.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "usb/usb_task.h"
#include "usb/tusb_config.h"

#include "utils/printf_color.h"


void task_blink(__unused void *pvParams) {
    bi_decl(bi_1pin_with_name(PICO_DEFAULT_LED_PIN, "On-board LED"));

    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    for (unsigned int i = 0;; i++) {
        gpio_put(PICO_DEFAULT_LED_PIN, 1);
        vTaskDelay(pdMS_TO_TICKS(300));
        gpio_put(PICO_DEFAULT_LED_PIN, 0);
        vTaskDelay(pdMS_TO_TICKS(700));

        printf_color(COLOR_WHITE, "i = %d\n", i);
    }
}

void board_init() {
    usbd_serial_init();
    set_sys_clock_khz(configCPU_CLOCK_HZ / 1000, false);
    stdio_init_all();
}

int main() {
    bi_decl(bi_program_description("ch32v003 debugger binary."));

    board_init();

    xTaskCreate(
            vTaskUsb,
            "usb",
            configMINIMAL_STACK_SIZE,
            NULL,
            1,
            NULL
    );

    xTaskCreate(
            task_blink,
            "default_led",
            configMINIMAL_STACK_SIZE * 4,
            NULL,
            1,
            NULL
    );

    vTaskStartScheduler();

    return 0;
}

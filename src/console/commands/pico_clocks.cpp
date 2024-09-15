#include "commands.h"

#include "FreeRTOSConfig.h"
#include "hardware/clocks.h"


static const struct {
    const clock_num_t index;
    const char *const description;
} clocks[] = {
        {clk_gpout0, "clk_gpout0 < GPIO Muxing 0"},
        {clk_gpout1, "clk_gpout1 < GPIO Muxing 1"},
        {clk_gpout2, "clk_gpout2 < GPIO Muxing 2"},
        {clk_gpout3, "clk_gpout3 < GPIO Muxing 3"},
        {clk_ref,    "clk_ref < Watchdog and timers reference clock"},
        {clk_sys,    "clk_sys < Processors, bus fabric, memory, memory mapped registers"},
        {clk_peri,   "clk_peri < Peripheral clock for UART and SPI"},
        {clk_usb,    "clk_usb < USB clock"},
        {clk_adc,    "clk_adc < ADC clock"},
        {clk_rtc,    "clk_rtc < Real time clock"},
};

static void sprint_human_hz(char *buf, uint32_t hz) {
    if (hz > 1000 * 1000) {
        sprintf(buf, "%.3f MHz", hz / 1000000.0);
    } else if (hz > 1000) {
        sprintf(buf, "%.3f KHz", hz / 1000.0);
    } else {
        sprintf(buf, "%lu Hz", hz);
    }
}

void command_clocks(Application &app, Console &c) {
    char freq_human[32];

    sprint_human_hz(freq_human, configCPU_CLOCK_HZ);
    printf("%12s | %s\n", freq_human, "FreeRTOS configCPU_CLOCK_HZ");

    for (auto item: clocks) {
        uint32_t clock_hz = clock_get_hz(item.index);

        sprint_human_hz(freq_human, clock_hz);
        printf("%12s | %s\n", freq_human, item.description);
    }
}


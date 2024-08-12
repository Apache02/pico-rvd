#include "ch32v003fun.h"
#include <stdio.h>
#include <stdbool.h>


#define WS2812_PIN      PC1


#define count_of(a)     (sizeof(a) / sizeof(a[0]))

#define RGB(r, g, b)    (((r & 0xFF) << 16) | ((g & 0xFF) << 8) | ((b & 0xFF) << 0))


void send_color(uint32_t color) {
    uint8_t grb[3] = {
            (color >> 8) & 0xFF,        // Green
            (color >> 16) & 0xFF,       // Red
            (color >> 0) & 0xFF,        // Blue
    };

    const uint8_t *pByte = grb;
    for (int bytes = count_of(grb); bytes > 0; bytes--) {
        uint8_t byte = *pByte++;

        for (int bits = 8; bits > 0; bits--) {
            if (byte & 0x80) {
                funDigitalWrite(WS2812_PIN, FUN_HIGH);
                DelaySysTick(25);
                funDigitalWrite(WS2812_PIN, FUN_LOW);
                DelaySysTick(1);
            } else {
                funDigitalWrite(WS2812_PIN, FUN_HIGH);
                asm volatile( "nop\nnop\nnop\nnop" );
                funDigitalWrite(WS2812_PIN, FUN_LOW);
                DelaySysTick(15);
            }

            byte <<= 1;
        }
    }
}

void send_colors(const uint32_t *colors, size_t length) {
    for (int i = 0; i < length; i++) {
        send_color(colors[i]);
    }
}

int main() {
    SystemInit();

    // Enable GPIOs
    funGpioInitAll();

    funPinMode(WS2812_PIN, GPIO_Speed_2MHz | GPIO_CNF_OUT_PP);

    static const uint32_t colors[] = {
            RGB(255, 0, 0),
            RGB(0, 255, 0),
            RGB(0, 0, 255),
            RGB(255, 128, 0),
            RGB(0, 255, 128),
            RGB(128, 0, 255),
            RGB(255, 0, 128),
            RGB(128, 255, 0),
            RGB(0, 128, 255),
            RGB(32, 32, 32),
            RGB(64, 64, 64),
            RGB(128, 128, 128),
    };

    while (1) {
        for (int i = 0; i < 12; i++) send_color(0);
        Delay_Ms(1);

        for (int ci = 0; ci < count_of(colors); ci++) {
            for (int i = 0; i < 12; i++) {
                for (int j = 0; j < 12; j++) {
                    send_color(j <= i ? colors[ci] : 0);
                }
                Delay_Ms(50);
            }
        }
    }
}

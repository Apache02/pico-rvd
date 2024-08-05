#include "ch32v003fun.h"
#include <stdio.h>

#define INTERVAL 150

void dot() {
    funDigitalWrite(PD6, FUN_HIGH);
    Delay_Ms(INTERVAL);
    funDigitalWrite(PD6, FUN_LOW);
    Delay_Ms(INTERVAL);
}

void dash() {
    funDigitalWrite(PD6, FUN_HIGH);
    Delay_Ms(INTERVAL * 3);
    funDigitalWrite(PD6, FUN_LOW);
    Delay_Ms(INTERVAL);
}

int main() {
    SystemInit();

    // Enable GPIOs
    funGpioInitAll();

    funPinMode(PD6, GPIO_Speed_10MHz | GPIO_CNF_OUT_PP);

    while (1) {
        dot();
        dot();
        dot();
        dash();
        dash();
        dash();
        dot();
        dot();
        dot();
        Delay_Ms(INTERVAL * 2);
    }
}

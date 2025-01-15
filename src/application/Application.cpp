#include "Application.h"

Application *gApp;

void Application::init() {
    const int ch32v003_flash_size = 16 * 1024;

    printf(COLOR_GREEN("// Starting PicoSWIO") "\n");
    swio = new PicoSWIO();
    swio->init(5); // pin on pico connected to swio pin on ch32
    swio->reset();

    printf(COLOR_GREEN("// Starting RVDebug") "\n");
    rvd = new RVDebug(swio, 16);
    rvd->init();

    printf(COLOR_GREEN("// Starting WCHFlash") "\n");
    flash = new WCHFlash(rvd, ch32v003_flash_size);
    flash->reset();
    //flash->dump();

    printf(COLOR_GREEN("// Starting SoftBreak") "\n");
    soft = new SoftBreak(rvd, flash);
    soft->init();
    //soft->dump();

    printf(COLOR_GREEN("// Starting GDBServer") "\n");
    gdb = new GDBServer(rvd, flash, soft);
    gdb->reset();
    //gdb->dump();
}

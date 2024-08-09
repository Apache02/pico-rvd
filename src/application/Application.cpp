#include "Application.h"

Application *gApp;

void Application::init() {
    const int ch32v003_flash_size = 16 * 1024;

    printf_g("// Starting PicoSWIO\n");
    swio = new PicoSWIO();
    swio->init(5); // pin on pico connected to swio pin on ch32
    swio->reset();

    printf_g("// Starting RVDebug\n");
    rvd = new RVDebug(swio, 16);
    rvd->init();

    printf_g("// Starting WCHFlash\n");
    flash = new WCHFlash(rvd, ch32v003_flash_size);
    flash->reset();
    //flash->dump();

    printf_g("// Starting SoftBreak\n");
    soft = new SoftBreak(rvd, flash);
    soft->init();
    //soft->dump();

    printf_g("// Starting GDBServer\n");
    gdb = new GDBServer(rvd, flash, soft);
    gdb->reset();
    //gdb->dump();

    printf_g("// Starting Console\n");
    console = new Console();
}

#pragma once

#include "Console.h"

#include "PicoSWIO.h"
#include "RVDebug.h"
#include "WCHFlash.h"
#include "SoftBreak.h"
#include "GDBServer.h"

class Application {
public:
    Console * console = nullptr;

    PicoSWIO *swio = nullptr;
    RVDebug *rvd = nullptr;
    WCHFlash *flash = nullptr;
    SoftBreak *soft = nullptr;
    GDBServer *gdb = nullptr;

    Application() {}

    void init();
};

extern Application *gApp;

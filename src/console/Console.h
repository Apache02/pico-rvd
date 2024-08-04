#pragma once

#include "Packet.h"
#include "RVDebug.h"

struct Console {
    Console(RVDebug *rvd);

    Console();

    ~Console();

    void reset();

    void dump();

    void start();

    void print_help();

    void update(bool ser_ie, char ser_in);

    bool dispatch_command();

    Packet packet;

    RVDebug *rvd = nullptr;
};

#pragma once

#include "Packet.h"

struct Console {
    Console();

    ~Console();

    void reset();

    void dump();

    void start();

    void print_help();

    void update(bool ser_ie, char ser_in);

    bool dispatch_command();

    Packet packet;
};

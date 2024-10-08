#include "Console.h"
#include "Application.h"

#include "utils.h"
#include "commands/commands.h"

#include <functional>
#include "pico/stdlib.h"

//------------------------------------------------------------------------------

Console::Console() {}

Console::~Console() {}

void Console::reset() {}

void Console::dump() {}

void Console::start() {
    printf_y("\n>> ");
}

//------------------------------------------------------------------------------

struct ConsoleHandler {
    const char *name;
    std::function<void(Application &, Console &)> handler;
};

ConsoleHandler handlers[] = {
        {
                "help",
                [](Application &app, Console &c) {
                    c.print_help();
                }
        },
        {
                "pico_clocks",
                command_clocks
        },
        {
                "init_swio",
                command_init_swio
        },
        {
                "reset",
                command_reset
        },
        {
                "halt",
                command_halt
        },
        {
                "resume",
                command_resume
        },
        {
                "step",
                command_step
        },
        {
                "dump",
                command_dump
        },
        {
                "dump2",
                command_dump2
        },
        {
                "status",
                command_status
        },
        {
                "part_id",
                command_part_id
        },
        {
                "chip_id",
                command_chip_id
        },
        {
                "wipe_chip",
                command_wipe
        },
        {
                "halt_on_reset",
                command_halt_on_reset
        },
};

static const int handler_count = sizeof(handlers) / sizeof(handlers[0]);

//------------------------------------------------------------------------------

void Console::print_help() {
    printf("Commands:\n");
    for (int i = 0; i < handler_count; i++) {
        printf("  %s\n", handlers[i].name);
    }
}

//------------------------------------------------------------------------------

bool Console::dispatch_command() {
    for (int i = 0; i < handler_count; i++) {
        auto &h = handlers[i];
        if (packet.match_word(h.name)) {
            h.handler(*gApp, *this);
            return true;
        }
    }
    printf_r("Command %s not handled\n", packet.buf);
    return false;
}

//------------------------------------------------------------------------------

void Console::update(bool ser_ie, char ser_in) {
    if (!ser_ie) return;

    if (ser_in == 0x7F /*backspace*/) {
        if (packet.cursor2 > packet.buf) {
            printf("\b \b");
            packet.cursor2--;
            packet.size--;
            *packet.cursor2 = 0;
        }
    } else if (ser_in == '\n' || ser_in == '\r') {
        *packet.cursor2 = 0;
        packet.cursor2 = packet.buf;

        printf("\n");

        if (packet.buf[0] != '\0') {
            uint32_t time_a = time_us_32();
            bool is_handled = dispatch_command();
            uint32_t time_b = time_us_32();

            if (is_handled) {
                printf("Command took %d us\n", time_b - time_a);
                if ((packet.cursor2 - packet.buf) < packet.size) {
                    printf_r("Leftover text in packet - {%s}\n", packet.cursor2);
                }
            }
        }

        packet.clear();
        printf_y(">> ");
    } else {
        printf("%c", ser_in);
        packet.put(ser_in);
    }
}

//------------------------------------------------------------------------------

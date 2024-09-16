#pragma once

#include "Console.h"
#include "Application.h"

void command_clocks(Application &app, Console &c);

void command_dump(Application &app, Console &c);

void command_dump2(Application &app, Console &c);

void command_reset(Application &app, Console &c);

void command_halt(Application &app, Console &c);

void command_resume(Application &app, Console &c);

void command_step(Application &app, Console &c);

void command_status(Application &app, Console &c);

void command_wipe(Application &app, Console &c);

void command_init_swio(Application &app, Console &c);

void command_part_id(Application &app, Console &c);

void command_halt_on_reset(Application &app, Console &c);

void command_chip_id(Application &app, Console &c);

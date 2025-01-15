#pragma once

#include "shell/Console.h"
#include "Application.h"

void command_clocks(Console &c);

void command_dump(Console &c);

void command_dump2(Console &c);

void command_reset(Console &c);

void command_halt(Console &c);

void command_resume(Console &c);

void command_step(Console &c);

void command_status(Console &c);

void command_wipe(Console &c);

void command_init_swio(Console &c);

void command_part_id(Console &c);

void command_halt_on_reset(Console &c);

void command_chip_id(Console &c);

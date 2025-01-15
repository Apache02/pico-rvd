#include "commands.h"


static inline bool check() {
    if (!gApp->rvd) {
        printf(COLOR_RED("rvd is null") "\n");
        return false;
    }

    if (!gApp->flash) {
        printf(COLOR_RED("flash is null") "\n");
        return false;
    }

    return true;
}

void command_wipe(Console &c) {
    if (!check()) return;

    gApp->flash->wipe_chip();
}

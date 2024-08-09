#include "commands.h"


static inline bool check(Application &app) {
    if (!app.rvd) {
        printf_r("rvd is null\n");
        return false;
    }

    if (!app.flash) {
        printf_r("flash is null\n");
        return false;
    }

    return true;
}

void command_wipe(Application &app, Console &c) {
    if (!check(app)) return;

    app.flash->wipe_chip();
}

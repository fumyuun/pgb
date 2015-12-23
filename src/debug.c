#include "debug.h"
#include "gfx.h"

debug_t debug;

void debug_init(gb_t *gb) {
    debug.show_tilemap = 0;
    debug.gb = gb;
}
void debug_tick() {
    static int i = 0;
    if (debug.show_tilemap && i++ > 1000) {
        gfx_draw_tilemap(&debug.gb->gfx);
        i = 0;
    }
}

void debug_toggle_tilemap() {
    debug.show_tilemap = !debug.show_tilemap;
}

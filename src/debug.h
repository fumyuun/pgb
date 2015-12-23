#ifndef DEBUG_H
#define DEBUG_H

#include "gb.h"

typedef struct _debug_t
{
    char show_tilemap;
    gb_t *gb;
} debug_t;

void debug_init(gb_t *gb);
void debug_tick();
void debug_toggle_tilemap();

extern debug_t debug;

#endif /* DEBUG_H */
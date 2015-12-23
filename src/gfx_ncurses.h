#ifndef GFX_H
#define GFX_H

#define SCREEN_H    144
#define SCREEN_W    160
//#define FRAMESKIP   4

#include <curses.h>

#include "mem.h"

typedef struct _gfx_t
{
    int *run;
    mem_t *mem;
    WINDOW *nc_win;
#ifdef FRAMESKIP
    unsigned int fscounter;
#endif
    unsigned int vscounter;
    unsigned long framecounter;
    uint8_t *lcdc;
    uint8_t *scy;
    uint8_t *scx;
    uint8_t *ly;
} gfx_t;

/* Initialise the GFX structure */
int gfx_init(gfx_t *gfx, mem_t *mem, int *run);
void gfx_tick(gfx_t *gfx);
inline void gfx_update(gfx_t *gfx);
void gfx_quit(gfx_t *gfx);

void gfx_draw_tilemap(gfx_t *gfx);

#endif

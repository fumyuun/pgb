#ifndef GFX_H
#define GFX_H

#define SCREEN_H    144
#define SCREEN_W    160
//#define FRAMESKIP   4

#include <SDL.h>

#include "mem.h"

typedef struct _gfx_t
{
    int *run;
    mem_t *mem;
    SDL_Window *sdl_window;
    SDL_Renderer *sdl_renderer;

    SDL_Window *tilemap_window;
    SDL_Renderer *tilemap_renderer;

    Uint32 black, white;
#ifdef FRAMESKIP
    unsigned int fscounter;
#endif
    unsigned int sleepcounter;
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

inline void sdl_putpixel(SDL_Renderer *renderer, int x, int y, Uint32 pixel);
#endif

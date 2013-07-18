#ifndef GFX_H
#define GFX_H

#ifdef SDL
    #ifdef NCURSES
        #error Both SDL and NCURSES defined!
    #endif
#endif

#define SCREEN_H    144
#define SCREEN_W    160
#define FRAMESKIP   4

#ifdef NCURSES
    #ifdef __CYGWIN__
        #include <ncurses/ncurses.h>
    #else
        #include <curses.h>
    #endif
#endif
#ifdef SDL
    #include <SDL/SDL.h>
#endif
#ifdef AALIB
    #include <aalib.h>
#endif

#include "mem.h"

typedef struct _gfx_t
{
    int *run;
    mem_t *mem;
#ifdef NCURSES
    WINDOW *nc_win;
#endif
#ifdef SDL
    SDL_Surface *sdl_screen;
    Uint32 black, white;
#endif
#ifdef AALIB
    aa_context *aal_context;
    aa_palette aal_pal;
#endif
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

#ifdef SDL
inline void sdl_putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
#endif
#endif

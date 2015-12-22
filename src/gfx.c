#include "main.h"

#ifdef NCURSES
    #define LYSCALER 2
#else
    #define LYSCALER 1
#endif

/*
 * Set the pixel at (x, y) to the given value. Assumes 8bpp surface!
 * NOTE: The surface must be locked before calling this!
 */
#ifdef SDL
void sdl_putpixel(SDL_Renderer *renderer, int x, int y, Uint32 pixel)
{
    SDL_SetRenderDrawColor(renderer, (pixel & 0xFF), (pixel & 0xFF), (pixel & 0xFF), SDL_ALPHA_OPAQUE);
    SDL_RenderDrawPoint(renderer, x, y);
}
#endif

/* Initialise the GFX structure */
int gfx_init(gfx_t *gfx, mem_t *mem, int *run)
{
    gfx->run = run;
    gfx->mem = mem;

#ifdef NCURSES
    initscr();
    gfx->nc_win = newwin(SCREEN_H, SCREEN_W, 0, 0);
    if(gfx->nc_win == NULL)             return -1;
    printf("Please ensure this terminal is %dx%d in size!\r\n",
            SCREEN_W, SCREEN_H);
#endif
#ifdef SDL
    if(SDL_Init(SDL_INIT_VIDEO) != 0)   return -2;
    gfx->sdl_window = SDL_CreateWindow("pgb", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_W, SCREEN_H, 0);
    gfx->sdl_renderer = SDL_CreateRenderer(gfx->sdl_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    gfx->black = 0x00;
    gfx->white = 0xFF;

    SDL_SetRenderDrawColor(gfx->sdl_renderer, 0xCC, 0xCC, 0xCC, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(gfx->sdl_renderer, NULL);
    SDL_RenderPresent(gfx->sdl_renderer);
#endif
#ifdef AALIB
    gfx->aal_context = aa_autoinit(&aa_defparams);
    if(gfx->aal_context == NULL)        return -3;
    if(aa_imgwidth(gfx->aal_context) < 2*SCREEN_W
    || aa_imgwidth(gfx->aal_context) < SCREEN_H) {
        aa_close(gfx->aal_context);
        return -3;
    }
#endif
#ifdef FRAMESKIP
    gfx->fscounter = 0;
#endif
    gfx->lcdc = mem_get_ioreg(gfx->mem, 0xFF40);
    gfx->scy = mem_get_ioreg(gfx->mem, 0xFF42);
    gfx->scx = mem_get_ioreg(gfx->mem, 0xFF43);
    gfx->ly = mem_get_ioreg(gfx->mem, 0xFF44);
    gfx->vscounter = 0;

    *gfx->ly = 0x90;

    return 0;
}

void gfx_tick(gfx_t *gfx)
{
    /* Vsync period is Every 17 ms, while the CPU tick every 238 ns.
       17ms / 238ns = 71429 */
    if(gfx->vscounter++ > 71429 / 0x100) {
        *gfx->ly += LYSCALER;
        if(*gfx->ly > 0x99) {
            *gfx->ly = 0;
            gfx->framecounter++;
        }
        gfx->vscounter = 0;
    }
#ifdef FRAMESKIP
    if(gfx->fscounter++ < FRAMESKIP)
        return;
    gfx->fscounter = 0;
#endif
    if((*gfx->lcdc & 0x80) == 0x80) {
        gfx_update(gfx);
    }

}

void gfx_update(gfx_t *gfx)
{
#ifdef NCURSES
    unsigned int px, py, tx;
    uint8_t *t, pl1, pl2, pl;

    if(*gfx->ly < 0x90) {
        py = *gfx->ly;
        move(py/LYSCALER, 0);
        t = gfx->mem->iram + 0x9800 + 32*(py/8 + *gfx->scy) + *gfx->scx/64;
        for(tx = 0; tx < 20; ++tx) {
            pl1 = gfx->mem->iram[0x8000 + 16 * *t + 2*(py % 8)];
            pl2 = gfx->mem->iram[0x8000 + 16 * *t + 2*(py % 8) + 1];
            pl = pl1 | pl2;
            for(px = 0x80; px != 0x00; px >>= 1) {
                if(pl & px) {
                    addch('X');
                }
                else
                    addch(' ');
            }
            t++;
        }
    }
    if(*gfx->ly == 0x90) {
        refresh();
    }
#endif

#ifdef SDL
    SDL_Event event;
    int px, py, tx, i;
    uint8_t *t, pl1, pl2, pl;
    if(*gfx->ly < 0x90) {
        py = *gfx->ly;
        t = gfx->mem->iram + 0x9800 + 32*(py/8 + *gfx->scy) + *gfx->scx/64;
        for(tx = 0; tx < 20; ++tx) {
            pl1 = gfx->mem->iram[0x8000 + 16 * *t + 2*(py % 8)];
            pl2 = gfx->mem->iram[0x8000 + 16 * *t + 2*(py % 8) + 1];
            pl = pl1 | pl2;
            i = 0x80;
            for(px = 0; px < 8; ++px) {
                if(pl & i) {
                    sdl_putpixel(gfx->sdl_renderer, tx*8 + px, py, gfx->black);
                }
                else
                    sdl_putpixel(gfx->sdl_renderer, tx*8 + px, py, gfx->white);
                i >>= 1;
            }
            t++;
        }
    }
    if(*gfx->ly == 0x90) {
        SDL_RenderPresent(gfx->sdl_renderer);
    }
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            *(gfx->run) = 0;
        }
    }
#endif

#ifdef AALIB
    int px, py, tx, i;
    uint8_t *t, pl1, pl2, pl;

    if(*gfx->ly < 0x90) {
        py = *gfx->ly;
        t = gfx->mem->iram + 0x9800 + 32*(py/8 + *gfx->scy) + *gfx->scx/64;
        for(tx = 0; tx < 20; ++tx) {
            pl1 = gfx->mem->iram[0x8000 + 16 * *t + 2*(py % 8)];
            pl2 = gfx->mem->iram[0x8000 + 16 * *t + 2*(py % 8) + 1];
            pl = pl1 | pl2;
            i = 0x80;
            for(px = 0; px < 8; ++px) {
                if(pl & i) {
                    aa_putpixel(gfx->aal_context, 2*(tx*8 + px), py, 0);
                    //aa_putpixel(gfx->aal_context, 2*(tx*8 + px)+1, py, 0);
                }
                else {
                    aa_putpixel(gfx->aal_context, 2*(tx*8 + px), py, 0xFF);
                    //aa_putpixel(gfx->aal_context, 2*(tx*8 + px)+1, py, 0xFF);
                }
                i >>= 1;
            }
            t++;
        }
    }
    if(*gfx->ly == 0x90) {
        aa_fastrender(gfx->aal_context, 0, 0, aa_scrwidth(gfx->aal_context), aa_scrheight(gfx->aal_context));
        aa_flush(gfx->aal_context);
    }
#endif
}

void gfx_quit(gfx_t *gfx)
{
#ifdef NCURSES
    endwin();
#endif

#ifdef SDL
    SDL_DestroyWindow(gfx->sdl_window);
    SDL_Quit();
#endif

#ifdef AALIB
    aa_close(gfx->aal_context);
#endif
}


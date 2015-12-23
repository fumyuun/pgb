#include "main.h"
#include "debug.h"

#define LYSCALER 1

/*
 * Set the pixel at (x, y) to the given value. Assumes 8bpp surface!
 * NOTE: The surface must be locked before calling this!
 */
void sdl_putpixel(SDL_Renderer *renderer, int x, int y, Uint32 pixel)
{
    SDL_SetRenderDrawColor(renderer, (pixel & 0xFF), (pixel & 0xFF), (pixel & 0xFF), SDL_ALPHA_OPAQUE);
    SDL_RenderDrawPoint(renderer, x, y);
}

/* Initialise the GFX structure */
int gfx_init(gfx_t *gfx, mem_t *mem, int *run)
{
    gfx->run = run;
    gfx->mem = mem;

    if(SDL_Init(SDL_INIT_VIDEO) != 0)   return -2;
    gfx->sdl_window = SDL_CreateWindow("pgb", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_W, SCREEN_H, 0);
    gfx->sdl_renderer = SDL_CreateRenderer(gfx->sdl_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    gfx->tilemap_window = SDL_CreateWindow("pgb-debug", 0, 0, 255, 255, 0);
    gfx->tilemap_renderer = SDL_CreateRenderer(gfx->tilemap_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    gfx->black = 0x00;
    gfx->white = 0xFF;

    SDL_SetRenderDrawColor(gfx->sdl_renderer, 0xCC, 0xCC, 0xCC, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(gfx->sdl_renderer, NULL);
    SDL_RenderPresent(gfx->sdl_renderer);

#ifdef FRAMESKIP
    gfx->fscounter = 0;
#endif
    gfx->lcdc = mem_get_ioreg(gfx->mem, 0xFF40);
    gfx->scy = mem_get_ioreg(gfx->mem, 0xFF42);
    gfx->scx = mem_get_ioreg(gfx->mem, 0xFF43);
    gfx->ly = mem_get_ioreg(gfx->mem, 0xFF44);
    gfx->vscounter = 0;
    gfx->sleepcounter = 0;

    *gfx->ly = 0x90;

    return 0;
}

void gfx_tick(gfx_t *gfx)
{
    SDL_Event event;
    /* Vsync period is Every 17 ms, while the CPU tick every 238 ns.
       17ms / 238ns = 71429 */
    if(gfx->vscounter++ > 71429 / 0x100) {
        *gfx->ly += LYSCALER;
        if(*gfx->ly > 0x99) {
            *gfx->ly = 0;
            gfx->framecounter++;
        }
        gfx->vscounter = 0;
        if((*gfx->lcdc & 0x80) == 0x80) {
            gfx_update(gfx);
        }
    }
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE)) {
            *gfx->run = 0;
        }
        if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_d) {
            debug_toggle_tilemap();
        }
    }
}

void gfx_update(gfx_t *gfx)
{
    int px, py, tx, i;
    uint8_t *t, pl1, pl2, pl;
    if(*gfx->ly < 0x90) {
        py = *gfx->ly;
        t = gfx->mem->iram + 0x9800 + 32*(py/8);
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
        #ifdef FRAMESKIP
            if(gfx->fscounter++ < FRAMESKIP) {
                return;
            }
            gfx->fscounter = 0;
        #endif
        SDL_RenderPresent(gfx->sdl_renderer);
    }
}

void gfx_quit(gfx_t *gfx)
{
    SDL_DestroyWindow(gfx->sdl_window);
    SDL_Quit();
}

void gfx_draw_tilemap(gfx_t *gfx) {
    int tx, ty, x, y;
    uint8_t *t;
    uint8_t pl1, pl2, pl;
    int mask;
    for (ty = 0; ty < 32; ++ty) {
        for (tx = 0; tx < 32; ++tx) {
            t = gfx->mem->iram + 0x9800 + 32 * ty + tx;
            for (y = 0; y < 8; ++y) {
                pl1 = gfx->mem->iram[0x8000 + 16 * *t + 2*y];
                pl2 = gfx->mem->iram[0x8000 + 16 * *t + 2*y + 1];
                pl = pl1 | pl2;
                mask = 0x80;
                for (x = 0; x < 8; ++x) {
                    if (pl & mask) {
                        sdl_putpixel(gfx->tilemap_renderer, tx * 8 + x, ty * 8 + y, gfx->black);
                    }
                    else {
                        sdl_putpixel(gfx->tilemap_renderer, tx * 8 + x, ty * 8 + y, gfx->white);
                    }
                    mask >>= 1;
                }
            }
        }

    }
    SDL_RenderPresent(gfx->tilemap_renderer);
}

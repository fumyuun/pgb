#include "main.h"

#ifdef SDL
#include <SDL.h>
Uint32 last_tick;
int cycles_since_tick;
#endif

/* Initialise the gameboy structure */
int gb_init(gb_t *gb, char *fname)
{
    int rc;
    gb->run = 1;
    if((rc = mem_init(&gb->mem, fname)) != 0)                 return -100+rc;
    if((rc = cpu_init(&gb->cpu, &gb->mem, &gb->run)) != 0)    return -200+rc;
    if((rc = gfx_init(&gb->gfx, &gb->mem, &gb->run)) != 0)    return -300+rc;

    #ifdef SDL

    last_tick = SDL_GetTicks();
    cycles_since_tick = 0;

    #endif

    return 0;
}
/* Run the gameboy one tick */
void gb_tick(gb_t *gb)
{
    cpu_tick(&gb->cpu);
    gfx_tick(&gb->gfx);
    ++cycles_since_tick;
    if (last_tick + 1000 < SDL_GetTicks()) {
        printf("%d ticks per second\n", cycles_since_tick);
        last_tick = SDL_GetTicks();
        cycles_since_tick = 0;
    }
}

void gb_quit(gb_t *gb)
{
    gfx_quit(&gb->gfx);
}

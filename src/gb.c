#include "main.h"

/* Initialise the gameboy structure */
int gb_init(gb_t *gb, char *fname)
{
    int rc;
    gb->run = 1;
    if((rc = mem_init(&gb->mem, fname)) != 0)                 return -100+rc;
    if((rc = cpu_init(&gb->cpu, &gb->mem, &gb->run)) != 0)    return -200+rc;
    if((rc = gfx_init(&gb->gfx, &gb->mem, &gb->run)) != 0)    return -300+rc;
    return 0;
}
/* Run the gameboy one tick */
void gb_tick(gb_t *gb)
{
    cpu_tick(&gb->cpu);
    gfx_tick(&gb->gfx);
}

void gb_quit(gb_t *gb)
{
    gfx_quit(&gb->gfx);
}

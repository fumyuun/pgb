#include "main.h"

/* Initialise the gameboy structure */
int gb_init(gb_t *gb, char *fname)
{
    int rc;
    gb->run = 0;
    if((rc = mem_init(&gb->mem, fname)) != 0)                 return -100+rc;
    if((rc = cpu_init(&gb->cpu, &gb->mem, &gb->run)) != 0)    return -200+rc;
    if((rc = gfx_init(&gb->gfx, &gb->mem, &gb->run)) != 0)    return -300+rc;
    return 0;
}
/* Run the gameboy until it terminates */
void gb_run(gb_t *gb)
{
    gb->run = 1;
    while(gb->run)
    {
        cpu_tick(&gb->cpu);
        gfx_tick(&gb->gfx);
    }
    gfx_quit(&gb->gfx);
}


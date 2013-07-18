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
    struct timespec tick;
    gb->run = 1;
    tick.tv_sec = 0;
    tick.tv_nsec = 238;     /* 4.194304 MHz ~ 238 ns*/
    while(gb->run)
    {
        cpu_tick(&gb->cpu);
        gfx_tick(&gb->gfx);
        //nanosleep(&tick, &tick);
    }
    gfx_quit(&gb->gfx);
}

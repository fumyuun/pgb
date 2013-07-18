#ifndef GB_H
#define GB_H

#include "cpu.h"
#include "gfx.h"
#include "mem.h"

/* The actual gameboy structure */
typedef struct _gb_t
{
    int run;
    cpu_t cpu;
    gfx_t gfx;
    mem_t mem;
} gb_t;

/* Initialise the gameboy structure */
int gb_init(gb_t *gb, char *fname);
/* Run the gameboy until it terminates */
void gb_run(gb_t *gb);

#endif

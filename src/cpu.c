#include "main.h"


/* Initialise the CPU structure */
int cpu_init(cpu_t *cpu, mem_t *mem, int *run)
{
    cpu->run = run;
    cpu->mem = mem;
    cpu->cycles = 0;
    return 0;
}

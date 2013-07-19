#ifndef MEM_H
#define MEM_H

#include <stdint.h>

typedef struct _mem_t
{
    uint8_t *bios_disabled;
    size_t bioslen;
    size_t romlen;
    size_t ramlen;
    uint8_t *bios;
    uint8_t *rom;
    uint8_t *ram;
    uint8_t iram[0xFFFF]; /* internal, video and IO */
} mem_t;

/* Initialise the memory structure */
int mem_init(mem_t *mem, char *fname);
uint8_t mem_read(mem_t *mem, uint16_t addr);
void mem_write(mem_t *mem, uint16_t addr, uint8_t val);
uint8_t *mem_get_ioreg(mem_t *mem, uint16_t addr);

#endif


#include "main.h"

/* Initialise the memory structure */
int mem_init(mem_t *mem, char *fname)
{
    FILE *bin, *rin;
    size_t len;
    char title[17];
    
    mem->bios_disabled = mem->iram + 0xFF50;
    
    bin = fopen("bios.bin", "rb");
    if(bin == NULL)
    {
        printf("Unable to open bios.bin\n");
        mem->bios = NULL;
        *mem->bios_disabled = 1;
    }
    else
    {
        fseek(bin, 0, SEEK_END);
        mem->bioslen = ftell(bin);
        rewind(bin);
        mem->bios = malloc(sizeof(uint8_t) * mem->bioslen);
        fread(mem->bios, sizeof(uint8_t), mem->bioslen, bin);
        fclose(bin);
        *mem->bios_disabled = 0;
    }
    rin = fopen(fname, "rb");
    if(rin == NULL)
    {
        printf("Unable to open %s\n", fname);
        mem->rom = NULL;
        return -1;
    }
    fseek(rin, 0, SEEK_END);
    len = ftell(rin);
    rewind(rin);
    mem->rom = malloc(sizeof(uint8_t) * (len > 0x8000 ? len : 0x8000));
    fread(mem->rom, sizeof(uint8_t), len, bin);
    mem->romlen = len > 0x8000 ? len : 0x8000;
    fclose(rin);
    
    strncpy(title, (char*)(mem->rom+0x134), 16);
#ifndef NCURSES
    printf("Succesfully opened %s <%s>\r\n", fname, title);
#endif
    mem->ramlen = 0x8000;
    mem->ram = malloc(sizeof(uint8_t) * mem->ramlen);
    
    return 0;
}

uint8_t mem_read(mem_t *mem, uint16_t addr)
{
    if(*mem->bios_disabled != 1 && addr < mem->bioslen)
        return mem->bios[addr];
    if(addr < 0x4000)
        return mem->rom[addr];
    else if(addr < 0x8000)
        return mem->rom[addr];   /* TODO rom banking */
    else
        return mem->iram[addr];

}

void mem_write(mem_t *mem, uint16_t addr, uint8_t val)
{
    if(*mem->bios_disabled != 1 && addr == 0xFF50 && val == 0x01) {
        #ifdef NCURSES
            mvprintw(1, 160, "Disabling bios!");
            refresh();
        #else
            printf("Disabling bios!\n");
        #endif
    }
        
    mem->iram[addr] = val;
}

uint8_t *mem_get_ioreg(mem_t *mem, uint16_t addr)
{
    return mem->iram + addr;
}

#include "main.h"

#define LYSCALER 2

/* Initialise the GFX structure */
int gfx_init(gfx_t *gfx, mem_t *mem, int *run)
{
    gfx->run = run;
    gfx->mem = mem;

    initscr();
    gfx->nc_win = newwin(SCREEN_H, SCREEN_W, 0, 0);
    if(gfx->nc_win == NULL)             return -1;
    printf("Please ensure this terminal is %dx%d in size!\r\n",
            SCREEN_W, SCREEN_H);

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
}

void gfx_quit(gfx_t *gfx)
{
    endwin();
}

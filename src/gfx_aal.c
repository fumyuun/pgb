#include "main.h"

#define LYSCALER 1

/* Initialise the GFX structure */
int gfx_init(gfx_t *gfx, mem_t *mem, int *run)
{
    gfx->run = run;
    gfx->mem = mem;

    gfx->aal_context = aa_autoinit(&aa_defparams);
    if(gfx->aal_context == NULL)        return -3;
    if(aa_imgwidth(gfx->aal_context) < 2*SCREEN_W
    || aa_imgwidth(gfx->aal_context) < SCREEN_H) {
        aa_close(gfx->aal_context);
        return -3;
    }
#ifdef FRAMESKIP
    gfx->fscounter = 0;
#endif
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
        if((*gfx->lcdc & 0x80) == 0x80) {
            gfx_update(gfx);
        }
    }
#ifdef FRAMESKIP
    if(gfx->fscounter++ < FRAMESKIP)
        return;
    gfx->fscounter = 0;
#endif

}

void gfx_update(gfx_t *gfx)
{
    int px, py, tx, i;
    uint8_t *t, pl1, pl2, pl;

    if(*gfx->ly < 0x90) {
        py = *gfx->ly;
        t = gfx->mem->iram + 0x9800 + 32*(py/8 + *gfx->scy) + *gfx->scx/64;
        for(tx = 0; tx < 20; ++tx) {
            pl1 = gfx->mem->iram[0x8000 + 16 * *t + 2*(py % 8)];
            pl2 = gfx->mem->iram[0x8000 + 16 * *t + 2*(py % 8) + 1];
            pl = pl1 | pl2;
            i = 0x80;
            for(px = 0; px < 8; ++px) {
                if(pl & i) {
                    aa_putpixel(gfx->aal_context, 2*(tx*8 + px), py, 0);
                    //aa_putpixel(gfx->aal_context, 2*(tx*8 + px)+1, py, 0);
                }
                else {
                    aa_putpixel(gfx->aal_context, 2*(tx*8 + px), py, 0xFF);
                    //aa_putpixel(gfx->aal_context, 2*(tx*8 + px)+1, py, 0xFF);
                }
                i >>= 1;
            }
            t++;
        }
    }
    if(*gfx->ly == 0x90) {
        aa_fastrender(gfx->aal_context, 0, 0, aa_scrwidth(gfx->aal_context), aa_scrheight(gfx->aal_context));
        aa_flush(gfx->aal_context);
    }
}

void gfx_quit(gfx_t *gfx)
{
    aa_close(gfx->aal_context);

}

void gfx_draw_tilemap(gfx_t *gfx) {

}
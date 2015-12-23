#include "main.h"
#include "debug.h"

int main(int argc, char **argv)
{
    gb_t gb;
    int rc;
    if(argc != 2)
    {
        printf("Usage: %s <rom file>\n", argv[0]);
        return -1;
    }

    rc = gb_init(&gb, argv[1]);

    if(rc != 0) {
        printf("Init failed: %d!\n", rc);
        return -2;
    }

    debug_init(&gb);

    while (gb.run) {
        gb_tick(&gb);
        debug_tick();
    }

    return 0;
}


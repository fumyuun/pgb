FILES=cpu.c gb.c gfx.c main.c mem.c instr.c
HEADERS=cpu.h gb.h gfx.h main.h mem.h instr.h

CC=/usr/bin/clang
INCLUDE_DIRS=-L/opt/local/lib -I/opt/local/include -L/usr/local/lib -I/usr/local/include
GLOBAL_FLAGS=-Wall -Wextra -pedantic -g -std=c99 -fsanitize=address $(INCLUDE_DIRS)

NCURSES_FLAGS=-lncurses -DNCURSES
SDL_FLAGS=`sdl2-config --cflags --libs` -DSDL
AAL_FLAGS=-laa -DAALIB

all:	../pgb-ncurses ../pgb-sdl ../pgb-aal

../pgb-ncurses:	$(FILES) $(HEADERS)
	$(CC) -o ../pgb-ncurses $(FILES) $(NCURSES_FLAGS) $(GLOBAL_FLAGS)

../pgb-sdl:	$(FILES) $(HEADERS)
	$(CC) -o ../pgb-sdl $(FILES) $(SDL_FLAGS) $(GLOBAL_FLAGS)

../pgb-aal:	$(FILES) $(HEADERS)
	$(CC) -o ../pgb-aal $(FILES) -lncurses $(AAL_FLAGS) $(GLOBAL_FLAGS)

clean:
	rm -f ../pgb-ncurses ../pgb-sdl ../pgb-aal

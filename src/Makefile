FILES=cpu.c gb.c gfx.c main.c mem.c instr.c

CC=/usr/bin/clang
INCLUDE_DIRS=-L/opt/local/lib -I/opt/local/lib -L/usr/local/lib -I/usr/local
GLOBAL_FLAGS=-Wall -Wextra -pedantic -g -std=c99 -fsanitize=address $(INCLUDE_DIRS)

NCURSES_FLAGS=-lncurses -DNCURSES
SDL_FLAGS=`sdl-config --cflags --libs` -DSDL
AAL_FLAGS=-laa -DAALIB

all:	../pgb-ncurses ../pgb-sdl ../pgb-aal

../pgb-ncurses:	$(FILES)
	$(CC) -o ../pgb-ncurses $(FILES) $(NCURSES_FLAGS) $(GLOBAL_FLAGS)

../pgb-sdl:	$(FILES)
	$(CC) -o ../pgb-sdl $(FILES) $(SDL_FLAGS) $(GLOBAL_FLAGS)

../pgb-aal:	$(FILES)
	$(CC) -o ../pgb-aal $(FILES) $(AAL_FLAGS) $(GLOBAL_FLAGS)

clean:
	rm -f ../pgb-ncurses ../pgb-sdl ../pgb-aal

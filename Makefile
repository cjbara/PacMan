all: pacman

pacman: pacman.c gfx3.c 
	gcc pacman.c gfx3.c -lm -lX11 -o pacman

clean:
	rm pacman

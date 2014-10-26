CC=gcc
CFLAGS=-W -Wall
CFLAGS=
LDFLAGS=
SDLFLAGS=-lSDL -lSDL_image -lSDL_gfx -lGL -lGLU
EXEC=asser_robot

POSTOPTIONS=-lm
FICHIERS=main.c PID.c asser.c communication.c hardware.c meca.c odometrie.c trajectoire.c\
         PID.h asser.h communication.h hardware.h meca.h odometrie.h trajectoire.h\

.PHONY:view

view: all
	./$(EXEC)

all: $(EXEC)

$(EXEC): main.c asser.o PID.o communication.o hardware.o odometrie.o trajectoire.o SDL/affichage.o
	$(CC) -o $@ $^ $(LDFLAGS) $(POSTOPTIONS) $(SDLFLAGS)

asser.o: asser.c asser.h PID.h trajectoire.h odometrie.h reglages.h
	$(CC) -o $@ -c $< $(CFLAGS)

PID.o: PID.c PID.h reglages.h
	$(CC) -o $@ -c $< $(CFLAGS)

communication.o: communication.c communication.h trajectoire.h
	$(CC) -o $@ -c $< $(CFLAGS)

hardware.o: hardware.c hardware.h
	$(CC) -o $@ -c $< $(CFLAGS)

odometrie.o: odometrie.c odometrie.h reglages.h hardware.h
	$(CC) -o $@ -c $< $(CFLAGS)

trajectoire.o: trajectoire.c trajectoire.h odometrie.h
	$(CC) -o $@ -c $< $(CFLAGS)

SDL/affichage.o: SDL/affichage.c SDL/affichage.h
	$(CC) -o $@ -c $< $(CFLAGS)

tarall: $(FICHIERS)
	tar -jcvf $(EXEC).tar.bz2 $^

clean:
	rm -f *.o

mrproper: clean
	rm -rf $(EXEC) $(EXEC).tar.bz2

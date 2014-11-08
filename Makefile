CC=gcc
CFLAGS=-W -Wall

LDFLAGS=-lm
SDLFLAGS=-lSDL -lSDL_image -lGL -lGLU -lSOIL
EXEC=asser_robot

FICHIERS_C=asser.c PID.c communication.c hardware.c odometrie.c trajectoire.c debug/affichage.c
FICHIERS_H=$(FICHIERS_C:.c=.h) reglages.h
FICHIERS_O=$(FICHIERS_C:.c=.o)

SOURCEFILES=main.c $(FICHIERS_C) $(FICHIERS_H) plateau.png

.PHONY:$(EXEC)

view: all
	./$(EXEC)

all: $(EXEC)

$(EXEC): main.c $(FICHIERS_O)
	$(CC) -o $@ $^ $(LDFLAGS) $(SDLFLAGS)

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

debug/affichage.o: debug/affichage.c debug/affichage.h
	$(CC) -o $@ -c $< $(CFLAGS)

tarall: $(SOURCEFILES)
	tar -jcvf $(EXEC).tar.bz2 $^

clean:
	rm -f $(FICHIERS_O)

mrproper: clean
	rm -rf $(EXEC) $(EXEC).tar.bz2

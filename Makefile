CC=gcc -fdiagnostics-color=auto -std=c99
CFLAGS=-W -Wall

LDFLAGS=-lm -lpthread
SDLFLAGS=-lSDL -lSDL_image -lGL -lGLU -lSOIL
EXEC=asser_robot

UART_DIR=../uart
COMMON_DIR=../common_header/

FICHIERS_C=asser.c PID.c communication.c hardware.c odometrie.c trajectoire.c debug/affichage.c math_precalc.c tests_unitaires.c $(UART_DIR)/text_reception.c reception.c match.c
COMMON_H=$(COMMON_DIR)/common.h
FICHIERS_H=$(FICHIERS_C:.c=.h) $(COMMON_H) reglages.h
FICHIERS_O=$(FICHIERS_C:.c=.o)

SOURCEFILES=main.c $(FICHIERS_C) $(FICHIERS_H) plateau.png

.PHONY:$(EXEC)

view: all
	./$(EXEC)

.PHONY:demo

demo: $(EXEC)
	sh ./slow_read.sh demo.txt | ./$(EXEC)

all: $(EXEC)

$(EXEC): main.c $(FICHIERS_O) $(COMMON_H)
	$(CC) -o $@ $^ $(LDFLAGS) $(SDLFLAGS)

asser.o: asser.c asser.h PID.h trajectoire.h odometrie.h reglages.h $(COMMON_H)
	$(CC) -o $@ -c $< $(CFLAGS)

PID.o: PID.c PID.h reglages.h $(COMMON_H)
	$(CC) -o $@ -c $< $(CFLAGS)

communication.o: communication.c communication.h trajectoire.h $(COMMON_H)
	$(CC) -o $@ -c $< $(CFLAGS)

hardware.o: hardware.c hardware.h $(COMMON_H)
	$(CC) -o $@ -c $< $(CFLAGS)

odometrie.o: odometrie.c odometrie.h reglages.h hardware.h math_precalc.h $(COMMON_H)
	$(CC) -o $@ -c $< $(CFLAGS)

trajectoire.o: trajectoire.c trajectoire.h odometrie.h asser.h $(COMMON_H)
	$(CC) -o $@ -c $< $(CFLAGS)

tests_unitaires.o: tests_unitaires.c tests_unitaires.h hardware.c asser.h odometrie.h communication.h reglages.h $(COMMON_H)
	$(CC) -o $@ -c $< $(CFLAGS)

math_precalc.o: math_precalc.c math_precalc.h $(COMMON_H)
	$(CC) -o $@ -c $< $(CFLAGS)

reception.o: reception.c reception.h communication.h $(COMMON_H)
	$(CC) -o $@ -c $< $(CFLAGS)

match.o: match.c match.h debug/affichage.h $(COMMON_H)
	$(CC) -o $@ -c $< $(CFLAGS)

debug/affichage.o: debug/affichage.c debug/affichage.h $(COMMON_H)
	$(CC) -o $@ -c $< $(CFLAGS)

tarall: $(SOURCEFILES)
	tar -jcvf $(EXEC).tar.bz2 $^

clean:
	rm -f $(FICHIERS_O)

mrproper: clean
	rm -rf $(EXEC) $(EXEC).tar.bz2

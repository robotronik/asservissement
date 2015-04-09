
# Options prises en compte :
#    SDL=yes       pour utiliser le simulateur
#    DEBUG=lvl     pour activer le debug
# NB : lvl = 1 à 3 selon le niveau souhaité, 3 étant le plus élevé

# Cibles :
#    run: lance l'executable
#    all: créer l'executable
#    demo: lance un ensemble de commande au robot

################################################################################

# Valeur par défaut

CC = gcc

CFLAGS  = -W -Wall -fdiagnostics-color=auto -std=c99
LDFLAGS = -lm -lpthread

EXEC  = asser_robot

# options
SDL   = no
DEBUG = 0

################################################################################

# Modules externe

UART_DIR        = ../uart
FICHIERS_UART_C = $(UART_DIR)/text_reception.c

COMMON_DIR =../common_code
COMMON_H   = $(COMMON_DIR)/*.h
FICHIER_AFFICHAGE_C = $(COMMON_DIR)/simulation/affichage.c


################################################################################

# Fichiers du projet

FICHIERS_C =\
	$(FICHIERS_UART_C) \
	asser.c \
	PID.c \
	communication.c \
	hardware.c \
	odometrie.c \
	trajectoire.c \
	math_precalc.c \
	tests_unitaires.c \
	reception.c \
	match.c

FICHIERS_H =\
	reglages.h

SOURCEFILES =\
	plateau.png \
	main.c

################################################################################

# Gestion des options

ifeq ($(SDL),yes)
	LDFLAGS    += -lSDL -lSDL_image -lGL -lGLU -lSOIL
	FICHIERS_C += $(FICHIER_AFFICHAGE_C)
	CFLAGS += -DUSE_SDL=1
endif

ifeq ($(DEBUG),0)
else
	CFLAGS += -DDEBUG=$(DEBUG) -g
endif

################################################################################

# Cibles du projet

FICHIERS_H  += $(FICHIERS_C:.c=.h) $(COMMON_H)
FICHIERS_O  += $(FICHIERS_C:.c=.o)
SOURCEFILES += $(FICHIERS_C) $(FICHIERS_H)

.PHONY:$(EXEC)
.PHONY:demo

run: all
	./$(EXEC)

demo: $(EXEC)
	sh ./slow_read.sh demo.txt | ./$(EXEC)

all: $(EXEC)

$(EXEC): main.c $(FICHIERS_O) $(COMMON_H)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(SDLFLAGS)

asser.o: PID.h trajectoire.h odometrie.h reglages.h

PID.o: reglages.h

communication.o: trajectoire.h

odometrie.o: reglages.h hardware.h math_precalc.h

trajectoire.o: odometrie.h asser.h

tests_unitaires.o: hardware.c asser.h odometrie.h communication.h reglages.h

reception.o: communication.h

match.o: 

%.o: %.c %.h $(COMMON_H)
	$(CC) $(CFLAGS) -o $@ -c $<

################################################################################

# Cibles génériques

tarall: $(SOURCEFILES)
	tar -jcvf $(EXEC).tar.bz2 $^

clean:
	rm -f $(FICHIERS_O) $(FICHIER_AFFICHAGE_C:.c=.o)
	# Ne pas oublié de supprimé les .o généré conditionnelement tel que ceux
	# qui dépendent de la SDL

mrproper: clean
	rm -rf $(EXEC) $(EXEC).tar.bz2


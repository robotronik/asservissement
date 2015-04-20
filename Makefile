
# Options prises en compte :
#    SDL=yes       pour utiliser le simulateur
#    DEBUG=lvl     pour activer le debug
# NB : lvl = 1 à 3 selon le niveau souhaité, 3 étant le plus élevé

# Cibles :
#    run: lance l'executable
#    all: créer l'executable
#    demo: lance un ensemble de commande au robot

################################################################################

# Options de compilation

export PC_EXEC    = asser_robot

export PC_CC      = gcc
export PC_CFLAGS  = -W -Wall -std=c99 -fdiagnostics-color=auto
export PC_LDFLAGS = $(PC_CFLAGS) -lm -lpthread

export PC_SDL_CF  = -DUSE_SDL=1
export PC_SDL_LDF = $(PC_SDL_CF) -lSDL -lSDL_image -lGL -lGLU -lSOIL

export PIC_ELF    = $(PC_EXEC).elf
export PIC_HEX    = $(PC_EXEC).hex
export PIC_CC     = /opt/xc16-toolchain-bin/bin/xc16-gcc
export PIC_ELF2HEX= /opt/xc16-toolchain-bin/bin/xc16-bin2hex

export PIC_CFLAGS = -DPIC_BUILD=1 -W -Wall -std=c99 -O0 -mcpu=33FJ128MC802 -omf=elf -msmart-io=1

export PIC_LDFLAGS= $(PIC_CFLAGS) -Wl,--script=p33FJ128MC802.gld,--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,--report-mem


# options
export PIC   = yes
export SDL   = yes
export DEBUG = no
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

ifeq ($(PIC), yes)
	EXEC    = $(PIC_ELF)
	CC      = $(PIC_CC)
	CFLAGS  = $(PIC_CFLAGS)
	LDFLAGS = $(PIC_LDFLAGS)
	FICHIERS_C += hardware_PIC.c
else
	EXEC    = $(PC_EXEC)
	CC      = $(PC_CC)
	CFLAGS  = $(PC_CFLAGS)
	LDFLAGS = $(PC_LDFLAGS)
	FICHIERS_C += hardware.c

	ifeq ($(SDL),yes)
		CFLAGS      += $(PC_SDL_CF)
		LDFLAGS     += $(PC_SDL_LDF)
		FICHIERS_C  += $(FICHIER_AFFICHAGE_C)
	endif

	ifeq ($(DEBUG),0)
	else
		CFLAGS += -DDEBUG=$(DEBUG) -g
	endif
endif

################################################################################

# Cibles du projet

FICHIERS_H  += $(FICHIERS_C:.c=.h) $(COMMON_H)
FICHIERS_O  += $(FICHIERS_C:.c=.o)
SOURCEFILES += $(FICHIERS_C) $(FICHIERS_H)


################################################################################
ifeq ($(PIC), yes)
# Compilation pour le PIC.

.PHONY:flash

flash:$(PIC_HEX)
	pk2cmd -P -Q -M -F$(PIC_HEX) -J -T

$(PIC_HEX):$(PIC_ELF)
	/opt/xc16-toolchain-bin/bin/xc16-bin2hex $^ -a -omf=elf

else
# Compilation pour le PC.

.PHONY:$(EXEC)

run: all
	./$(EXEC)

demo: $(EXEC)
	sh ./slow_read.sh demo.txt | ./$(EXEC)

endif

all: $(EXEC)

$(EXEC): main.c $(FICHIERS_O) $(COMMON_H)
	$(CC) -o $@ $^ $(LDFLAGS) $(SDLFLAGS)

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


# Cibles :
#    run: lance l'executable
#    all: créer l'executable
#    demo: lance un ensemble de commande au robot
################################################################################

# Options
export PIC   = no
export ROBOT = gros
export SDL   = yes
export DEBUG = 1

# Constantes de compilation

export PC_EXEC    = asser_robot

export PC_CC      = gcc
export PC_CFLAGS  = -DPIC_BUILD=0 -W -Wall -std=c99 -fdiagnostics-color=auto
export PC_LDFLAGS = -lm -lpthread

export PC_SDL_CF  = -DUSE_SDL=1
export PC_SDL_LDF = -lSDL -lSDL_image -lGL -lGLU -lSOIL

export PIC_ELF    = $(PC_EXEC).elf
export PIC_HEX    = $(PC_EXEC).hex
export PIC_CC     = /opt/xc16-toolchain-bin/bin/xc16-gcc
export PIC_ELF2HEX= /opt/xc16-toolchain-bin/bin/xc16-bin2hex

export PIC_CFLAGS = -DPIC_BUILD=1 -W -Wall -std=c99 -O0 -mcpu=33FJ128MC802 -omf=elf -msmart-io=1

export PIC_LDFLAGS= -Wl,--script=p33FJ128MC802.gld,--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,--report-mem

################################################################################

# Modules externe

COMMON_DIR = ../common_code/
COMMON_H   = $(COMMON_DIR)/*.h
UART_DIR   = $(COMMON_DIR)/uart/
FICHIERS_UART_C = $(UART_DIR)/text_reception.c

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

#FICHIERS_H =\
#	reglages.h

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

	F_HARDWARE_C = hardware_PIC.c
	F_REGLAGES_H = reglages_PC.h

	ifeq ($(ROBOT),petit)
		CFLAGS  += -DGROS=0 -DPETIT=1
	else
		CFLAGS  += -DGROS=1 -DPETIT=0
	endif

else
	EXEC    = $(PC_EXEC)
	CC      = $(PC_CC)
	CFLAGS  = $(PC_CFLAGS)
	LDFLAGS = $(PC_LDFLAGS)

	F_HARDWARE_C = hardware_PC.c
	F_REGLAGES_H = reglages_PC.h

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
FICHIERS_C  += $(F_HARDWARE_C)

FICHIERS_H  += $(FICHIERS_C:.c=.h) hardware.h $(F_REGLAGES_H) $(COMMON_H)
FICHIERS_O  += $(FICHIERS_C:.c=.o) main.o
SOURCEFILES += $(FICHIERS_C) $(FICHIERS_H)

all:$(EXEC)
.PHONY: all

################################################################################
.PHONY: flash run demo
ifeq ($(PIC), yes)
# Exécution pour le PIC.
$(PIC_HEX):$(PIC_ELF)
	/opt/xc16-toolchain-bin/bin/xc16-bin2hex $^ -a -omf=elf

flash:$(PIC_HEX)
	pk2cmd -P -M -F$(PIC_HEX) -J -T

else
# Exécution pour le PC.
run: all
	./$(EXEC)

demo: $(EXEC)
	sh ./slow_read.sh demo.txt | ./$(EXEC)

endif

################################################################################

# Compilation

$(EXEC): $(FICHIERS_O)
	$(CC) -o $@ $^ $(LDFLAGS) $(SDLFLAGS)

asser.o: PID.h trajectoire.h odometrie.h $(F_REGLAGES_H)

PID.o: $(F_REGLAGES_H)

communication.o: trajectoire.h

odometrie.o: $(F_REGLAGES_H) hardware.h math_precalc.h

trajectoire.o: odometrie.h asser.h

tests_unitaires.o: $(F_HARDWARE_C) asser.h odometrie.h communication.h $(F_REGLAGES_H)

reception.o: communication.h

match.o:

%.o: %.c $(COMMON_H)
	$(CC) $(CFLAGS) -o $@ -c $<

################################################################################

# Cibles génériques
.PHONY:tarall clean mrproper

tarall: $(SOURCEFILES)
	tar -jcvf $(EXEC).tar.bz2 $^

clean:
	rm -f $(FICHIERS_O) $(FICHIER_AFFICHAGE_C:.c=.o)

mrproper: clean
	rm -rf $(EXEC) $(PIC_ELF) $(PIC_HEX) $(EXEC).tar.bz2


PROJECT=asservissement
default: asservissement
# Default Options
export ARCH  ?= dsPIC33F
export DEBUG ?= _WARNING_
export ROBOT ?= gros

PARENT_DIR = ../

# Constantes de compilation
EXEC    = asser_robot
include $(PARENT_DIR)/hardware/common.rules
################################################################################
# Fichiers du projet

FICHIERS_C =\
	asser.c \
	PID.c \
	odometrie.c \
	trajectoire.c \
	maths_utils.c \
	tests_unitaires.c \

# Fichier de réglages dépendant de la plateforme
ifeq ($(ARCH), PC)
	REGLAGES_H = reglages/$(ARCH).h
else
	REGLAGES_H = reglages/$(ROBOT)_robot.h
endif

CFLAGS += -DREGLAGES_HEADER='"$(REGLAGES_H)"'

# Fichier de hardware dépendant de l'architecture
HARDWARE_C = hardware_$(ARCH).c
FICHIERS_C+= $(HARDWARE_C)

FICHIERS_O  += $(addprefix $(BUILD_DIR)/, $(FICHIERS_C:.c=.o) )

################################################################################
# Compilation

# Règles de compilation de la "librairie asser"
_libAsser: $(BUILD_DIR)/libAsser.a

$(BUILD_DIR)/libAsser.a: $(FICHIERS_O)

# Règles de compilation de l'exécutable asser
asservissement:$(BUILD_DIR)/$(EXEC)

$(BUILD_DIR)/$(EXEC): $(BUILD_DIR)/main.o libAsser libCommAsser libHardware
	@echo "	CC	$(PROJECT)|$@"
	@$(CC) $(CFLAGS) -o $@ $(BUILD_DIR)/main.o -lAsser -lCommAsser -lHardware $(LDFLAGS)

# Dépendances en headers, pas utile en réalité, mais mieux
$(BUILD_DIR)/asser.o: asser.h odometrie.h PID.h trajectoire.h # $(REGLAGES_H)
$(BUILD_DIR)/PID.o: PID.h $(REGLAGES_H)
$(BUILD_DIR)/odometrie.o: odometrie.h $(REGLAGES_H)
$(BUILD_DIR)/trajectoire.o: asser.h odometrie.h trajectoire.h
$(BUILD_DIR)/tests_unitaires.o: tests_unitaires.h asser.h odometrie.h $(REGLAGES_H)
$(BUILD_DIR)/hardware.o: $(HARDWARE_C) hardware.h

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

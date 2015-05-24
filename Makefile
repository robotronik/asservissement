PROJECT=asservissement

# Default Options
export ARCH  = PC
export ROBOT = gros
export SDL   = yes
export DEBUG = _WARNING_

export PARENT_DIR = ../
include $(PARENT_DIR)/common_code/common.mk

# Constantes de compilation
EXEC    = asser_robot
PIC_ELF = $(EXEC).elf
PIC_HEX = $(EXEC).hex

################################################################################
# Fichiers du projet

FICHIERS_C =\
	asser.c \
	PID.c \
	odometrie.c \
	trajectoire.c \
	math_precalc.c \
	tests_unitaires.c \
	match.c \

# Fichier de réglages dépendant de la plateforme
REGLAGES_H = reglages.h
ifeq ($(ARCH), PC)
	REGLAGES_H += reglages/$(ARCH).h
else
	REGLAGES_H += reglages/$(ROBOT)_robot.h
endif

FICHIERS_H = hardware.h $(REGLAGES_H) $(FICHIERS_C:.c=.h)

# Fichier de hardware dépendant de l'architecture
HARDWARE_C = hardware_$(ARCH).c
FICHIERS_C+= $(HARDWARE_C) main.c

################################################################################
# Gestion des options

FICHIERS_O  += $(addprefix $(BUILD_DIR)/, $(FICHIERS_C:.c=.o) )
################################################################################

# Cibles du projet

.PHONY: all

################################################################################
.PHONY: flash run demo
ifeq ($(ARCH), dsPIC)
# Exécution pour le PIC.
$(PIC_HEX):$(EXEC)
	@echo "Converting to Intel HEX Format…"
	@/opt/xc16-toolchain-bin/bin/xc16-bin2hex $^ -a -omf=elf
	@echo "Done !"

flash:$(PIC_HEX)
	pk2cmd -P -M -F$(PIC_HEX) -J -T
endif


ifeq ($(ARCH), PC)
# Exécution pour le PC.
run: all
	./$(EXEC)

demo: $(EXEC)
	sh ./slow_read.sh demo.txt | ./$(EXEC)

endif
all:$(EXEC)

################################################################################

# Compilation

$(EXEC): $(FICHIERS_O) $(COMMON_DIR)/$(BUILD_DIR)/libCommon.a $(COMMUNICATION_DIR)/$(BUILD_DIR)/libCommAsser.a
	@$(CC) -o $@ $^ $(LDFLAGS) $(SDLFLAGS)

# Dépendances en headers, pas utile en réalité, mais mieux
$(BUILD_DIR)/asser.o: asser.h odometrie.h PID.h trajectoire.h # $(REGLAGES_H)
$(BUILD_DIR)/PID.o: PID.h $(REGLAGES_H)
$(BUILD_DIR)/odometrie.o: odometrie.h $(REGLAGES_H)
$(BUILD_DIR)/trajectoire.o: asser.h odometrie.h trajectoire.h
$(BUILD_DIR)/tests_unitaires.o: tests_unitaires.h asser.h odometrie.h $(REGLAGES_H)
$(BUILD_DIR)/hardware.o:
$(BUILD_DIR)/match.o: match.h

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	@echo "	CC	$(PROJECT)|$(notdir $@)"
	@$(CC) $(CFLAGS) -o $@ -c $<

$(BUILD_DIR):
	@mkdir $(BUILD_DIR) $ -p

# Librairies
.PHONY: $(COMMUNICATION_OBJ_DIR)/comm_asser.a $(LIBCOMMON_OBJ_DIR)/libCommon.a

$(COMMUNICATION_DIR)/$(BUILD_DIR)/libCommAsser.a:
	@$(MAKE) ARCH=$(ARCH) ROBOT=$(ROBOT) SDL=$(SDL) DEBUG=$(DEBUG) -C $(COMMUNICATION_DIR) libCommAsser

$(COMMON_DIR)/$(BUILD_DIR)/libCommon.a:
	@$(MAKE) ARCH=$(ARCH) ROBOT=$(ROBOT) SDL=$(SDL) DEBUG=$(DEBUG) -C $(COMMON_DIR) libCommon

################################################################################
# Cibles génériques

.PHONY:tarall clean mrproper

clean:
	@echo "Cleaning $(PROJECT) directory…"
	@find $(BUILD_DIR) -name '*.o' -delete
	@rmdir -p --ignore-fail-on-non-empty $(BUILD_DIR)/*/* 2>/dev/null || true

mrproper: clean
	@echo "Hard-cleaning  $(PROJECT) directory…"
	@rm -rf $(EXEC) $(PIC_ELF) $(PIC_HEX) $(EXEC).tar.bz2
	@$(MAKE) clean -C $(COMMUNICATION_DIR)
	@$(MAKE) clean -C $(COMMON_DIR)


#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-Robot.mk)" "nbproject/Makefile-local-Robot.mk"
include nbproject/Makefile-local-Robot.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=Robot
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/Asservissement.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/Asservissement.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../asservissement/asser.c ../asservissement/communication.c ../asservissement/hardware_PIC.c ../asservissement/main.c ../asservissement/match.c ../asservissement/math_precalc.c ../asservissement/odometrie.c ../asservissement/PID.c ../asservissement/tests_unitaires.c ../asservissement/trajectoire.c ../common_code/uart/protocole.c ../common_code/uart/reception.c ../common_code/uart/text_reception.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1141334063/asser.o ${OBJECTDIR}/_ext/1141334063/communication.o ${OBJECTDIR}/_ext/1141334063/hardware_PIC.o ${OBJECTDIR}/_ext/1141334063/main.o ${OBJECTDIR}/_ext/1141334063/match.o ${OBJECTDIR}/_ext/1141334063/math_precalc.o ${OBJECTDIR}/_ext/1141334063/odometrie.o ${OBJECTDIR}/_ext/1141334063/PID.o ${OBJECTDIR}/_ext/1141334063/tests_unitaires.o ${OBJECTDIR}/_ext/1141334063/trajectoire.o ${OBJECTDIR}/_ext/1299978293/protocole.o ${OBJECTDIR}/_ext/1299978293/reception.o ${OBJECTDIR}/_ext/1299978293/text_reception.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1141334063/asser.o.d ${OBJECTDIR}/_ext/1141334063/communication.o.d ${OBJECTDIR}/_ext/1141334063/hardware_PIC.o.d ${OBJECTDIR}/_ext/1141334063/main.o.d ${OBJECTDIR}/_ext/1141334063/match.o.d ${OBJECTDIR}/_ext/1141334063/math_precalc.o.d ${OBJECTDIR}/_ext/1141334063/odometrie.o.d ${OBJECTDIR}/_ext/1141334063/PID.o.d ${OBJECTDIR}/_ext/1141334063/tests_unitaires.o.d ${OBJECTDIR}/_ext/1141334063/trajectoire.o.d ${OBJECTDIR}/_ext/1299978293/protocole.o.d ${OBJECTDIR}/_ext/1299978293/reception.o.d ${OBJECTDIR}/_ext/1299978293/text_reception.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1141334063/asser.o ${OBJECTDIR}/_ext/1141334063/communication.o ${OBJECTDIR}/_ext/1141334063/hardware_PIC.o ${OBJECTDIR}/_ext/1141334063/main.o ${OBJECTDIR}/_ext/1141334063/match.o ${OBJECTDIR}/_ext/1141334063/math_precalc.o ${OBJECTDIR}/_ext/1141334063/odometrie.o ${OBJECTDIR}/_ext/1141334063/PID.o ${OBJECTDIR}/_ext/1141334063/tests_unitaires.o ${OBJECTDIR}/_ext/1141334063/trajectoire.o ${OBJECTDIR}/_ext/1299978293/protocole.o ${OBJECTDIR}/_ext/1299978293/reception.o ${OBJECTDIR}/_ext/1299978293/text_reception.o

# Source Files
SOURCEFILES=../asservissement/asser.c ../asservissement/communication.c ../asservissement/hardware_PIC.c ../asservissement/main.c ../asservissement/match.c ../asservissement/math_precalc.c ../asservissement/odometrie.c ../asservissement/PID.c ../asservissement/tests_unitaires.c ../asservissement/trajectoire.c ../common_code/uart/protocole.c ../common_code/uart/reception.c ../common_code/uart/text_reception.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-Robot.mk dist/${CND_CONF}/${IMAGE_TYPE}/Asservissement.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=33FJ128MC802
MP_LINKER_FILE_OPTION=,--script=p33FJ128MC802.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1141334063/asser.o: ../asservissement/asser.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1141334063" 
	@${RM} ${OBJECTDIR}/_ext/1141334063/asser.o.d 
	@${RM} ${OBJECTDIR}/_ext/1141334063/asser.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../asservissement/asser.c  -o ${OBJECTDIR}/_ext/1141334063/asser.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1141334063/asser.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1    -omf=elf -DPIC_BUILD=1 -DGROS=1 -std=c99 -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1141334063/asser.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1141334063/communication.o: ../asservissement/communication.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1141334063" 
	@${RM} ${OBJECTDIR}/_ext/1141334063/communication.o.d 
	@${RM} ${OBJECTDIR}/_ext/1141334063/communication.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../asservissement/communication.c  -o ${OBJECTDIR}/_ext/1141334063/communication.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1141334063/communication.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1    -omf=elf -DPIC_BUILD=1 -DGROS=1 -std=c99 -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1141334063/communication.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1141334063/hardware_PIC.o: ../asservissement/hardware_PIC.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1141334063" 
	@${RM} ${OBJECTDIR}/_ext/1141334063/hardware_PIC.o.d 
	@${RM} ${OBJECTDIR}/_ext/1141334063/hardware_PIC.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../asservissement/hardware_PIC.c  -o ${OBJECTDIR}/_ext/1141334063/hardware_PIC.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1141334063/hardware_PIC.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1    -omf=elf -DPIC_BUILD=1 -DGROS=1 -std=c99 -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1141334063/hardware_PIC.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1141334063/main.o: ../asservissement/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1141334063" 
	@${RM} ${OBJECTDIR}/_ext/1141334063/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1141334063/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../asservissement/main.c  -o ${OBJECTDIR}/_ext/1141334063/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1141334063/main.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1    -omf=elf -DPIC_BUILD=1 -DGROS=1 -std=c99 -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1141334063/main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1141334063/match.o: ../asservissement/match.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1141334063" 
	@${RM} ${OBJECTDIR}/_ext/1141334063/match.o.d 
	@${RM} ${OBJECTDIR}/_ext/1141334063/match.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../asservissement/match.c  -o ${OBJECTDIR}/_ext/1141334063/match.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1141334063/match.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1    -omf=elf -DPIC_BUILD=1 -DGROS=1 -std=c99 -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1141334063/match.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1141334063/math_precalc.o: ../asservissement/math_precalc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1141334063" 
	@${RM} ${OBJECTDIR}/_ext/1141334063/math_precalc.o.d 
	@${RM} ${OBJECTDIR}/_ext/1141334063/math_precalc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../asservissement/math_precalc.c  -o ${OBJECTDIR}/_ext/1141334063/math_precalc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1141334063/math_precalc.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1    -omf=elf -DPIC_BUILD=1 -DGROS=1 -std=c99 -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1141334063/math_precalc.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1141334063/odometrie.o: ../asservissement/odometrie.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1141334063" 
	@${RM} ${OBJECTDIR}/_ext/1141334063/odometrie.o.d 
	@${RM} ${OBJECTDIR}/_ext/1141334063/odometrie.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../asservissement/odometrie.c  -o ${OBJECTDIR}/_ext/1141334063/odometrie.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1141334063/odometrie.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1    -omf=elf -DPIC_BUILD=1 -DGROS=1 -std=c99 -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1141334063/odometrie.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1141334063/PID.o: ../asservissement/PID.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1141334063" 
	@${RM} ${OBJECTDIR}/_ext/1141334063/PID.o.d 
	@${RM} ${OBJECTDIR}/_ext/1141334063/PID.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../asservissement/PID.c  -o ${OBJECTDIR}/_ext/1141334063/PID.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1141334063/PID.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1    -omf=elf -DPIC_BUILD=1 -DGROS=1 -std=c99 -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1141334063/PID.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1141334063/tests_unitaires.o: ../asservissement/tests_unitaires.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1141334063" 
	@${RM} ${OBJECTDIR}/_ext/1141334063/tests_unitaires.o.d 
	@${RM} ${OBJECTDIR}/_ext/1141334063/tests_unitaires.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../asservissement/tests_unitaires.c  -o ${OBJECTDIR}/_ext/1141334063/tests_unitaires.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1141334063/tests_unitaires.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1    -omf=elf -DPIC_BUILD=1 -DGROS=1 -std=c99 -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1141334063/tests_unitaires.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1141334063/trajectoire.o: ../asservissement/trajectoire.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1141334063" 
	@${RM} ${OBJECTDIR}/_ext/1141334063/trajectoire.o.d 
	@${RM} ${OBJECTDIR}/_ext/1141334063/trajectoire.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../asservissement/trajectoire.c  -o ${OBJECTDIR}/_ext/1141334063/trajectoire.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1141334063/trajectoire.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1    -omf=elf -DPIC_BUILD=1 -DGROS=1 -std=c99 -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1141334063/trajectoire.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1299978293/protocole.o: ../common_code/uart/protocole.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1299978293" 
	@${RM} ${OBJECTDIR}/_ext/1299978293/protocole.o.d 
	@${RM} ${OBJECTDIR}/_ext/1299978293/protocole.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../common_code/uart/protocole.c  -o ${OBJECTDIR}/_ext/1299978293/protocole.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1299978293/protocole.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1    -omf=elf -DPIC_BUILD=1 -DGROS=1 -std=c99 -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1299978293/protocole.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1299978293/reception.o: ../common_code/uart/reception.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1299978293" 
	@${RM} ${OBJECTDIR}/_ext/1299978293/reception.o.d 
	@${RM} ${OBJECTDIR}/_ext/1299978293/reception.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../common_code/uart/reception.c  -o ${OBJECTDIR}/_ext/1299978293/reception.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1299978293/reception.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1    -omf=elf -DPIC_BUILD=1 -DGROS=1 -std=c99 -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1299978293/reception.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1299978293/text_reception.o: ../common_code/uart/text_reception.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1299978293" 
	@${RM} ${OBJECTDIR}/_ext/1299978293/text_reception.o.d 
	@${RM} ${OBJECTDIR}/_ext/1299978293/text_reception.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../common_code/uart/text_reception.c  -o ${OBJECTDIR}/_ext/1299978293/text_reception.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1299978293/text_reception.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1    -omf=elf -DPIC_BUILD=1 -DGROS=1 -std=c99 -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1299978293/text_reception.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
else
${OBJECTDIR}/_ext/1141334063/asser.o: ../asservissement/asser.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1141334063" 
	@${RM} ${OBJECTDIR}/_ext/1141334063/asser.o.d 
	@${RM} ${OBJECTDIR}/_ext/1141334063/asser.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../asservissement/asser.c  -o ${OBJECTDIR}/_ext/1141334063/asser.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1141334063/asser.o.d"        -g -omf=elf -DPIC_BUILD=1 -DGROS=1 -std=c99 -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1141334063/asser.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1141334063/communication.o: ../asservissement/communication.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1141334063" 
	@${RM} ${OBJECTDIR}/_ext/1141334063/communication.o.d 
	@${RM} ${OBJECTDIR}/_ext/1141334063/communication.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../asservissement/communication.c  -o ${OBJECTDIR}/_ext/1141334063/communication.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1141334063/communication.o.d"        -g -omf=elf -DPIC_BUILD=1 -DGROS=1 -std=c99 -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1141334063/communication.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1141334063/hardware_PIC.o: ../asservissement/hardware_PIC.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1141334063" 
	@${RM} ${OBJECTDIR}/_ext/1141334063/hardware_PIC.o.d 
	@${RM} ${OBJECTDIR}/_ext/1141334063/hardware_PIC.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../asservissement/hardware_PIC.c  -o ${OBJECTDIR}/_ext/1141334063/hardware_PIC.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1141334063/hardware_PIC.o.d"        -g -omf=elf -DPIC_BUILD=1 -DGROS=1 -std=c99 -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1141334063/hardware_PIC.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1141334063/main.o: ../asservissement/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1141334063" 
	@${RM} ${OBJECTDIR}/_ext/1141334063/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1141334063/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../asservissement/main.c  -o ${OBJECTDIR}/_ext/1141334063/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1141334063/main.o.d"        -g -omf=elf -DPIC_BUILD=1 -DGROS=1 -std=c99 -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1141334063/main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1141334063/match.o: ../asservissement/match.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1141334063" 
	@${RM} ${OBJECTDIR}/_ext/1141334063/match.o.d 
	@${RM} ${OBJECTDIR}/_ext/1141334063/match.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../asservissement/match.c  -o ${OBJECTDIR}/_ext/1141334063/match.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1141334063/match.o.d"        -g -omf=elf -DPIC_BUILD=1 -DGROS=1 -std=c99 -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1141334063/match.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1141334063/math_precalc.o: ../asservissement/math_precalc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1141334063" 
	@${RM} ${OBJECTDIR}/_ext/1141334063/math_precalc.o.d 
	@${RM} ${OBJECTDIR}/_ext/1141334063/math_precalc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../asservissement/math_precalc.c  -o ${OBJECTDIR}/_ext/1141334063/math_precalc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1141334063/math_precalc.o.d"        -g -omf=elf -DPIC_BUILD=1 -DGROS=1 -std=c99 -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1141334063/math_precalc.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1141334063/odometrie.o: ../asservissement/odometrie.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1141334063" 
	@${RM} ${OBJECTDIR}/_ext/1141334063/odometrie.o.d 
	@${RM} ${OBJECTDIR}/_ext/1141334063/odometrie.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../asservissement/odometrie.c  -o ${OBJECTDIR}/_ext/1141334063/odometrie.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1141334063/odometrie.o.d"        -g -omf=elf -DPIC_BUILD=1 -DGROS=1 -std=c99 -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1141334063/odometrie.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1141334063/PID.o: ../asservissement/PID.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1141334063" 
	@${RM} ${OBJECTDIR}/_ext/1141334063/PID.o.d 
	@${RM} ${OBJECTDIR}/_ext/1141334063/PID.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../asservissement/PID.c  -o ${OBJECTDIR}/_ext/1141334063/PID.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1141334063/PID.o.d"        -g -omf=elf -DPIC_BUILD=1 -DGROS=1 -std=c99 -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1141334063/PID.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1141334063/tests_unitaires.o: ../asservissement/tests_unitaires.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1141334063" 
	@${RM} ${OBJECTDIR}/_ext/1141334063/tests_unitaires.o.d 
	@${RM} ${OBJECTDIR}/_ext/1141334063/tests_unitaires.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../asservissement/tests_unitaires.c  -o ${OBJECTDIR}/_ext/1141334063/tests_unitaires.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1141334063/tests_unitaires.o.d"        -g -omf=elf -DPIC_BUILD=1 -DGROS=1 -std=c99 -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1141334063/tests_unitaires.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1141334063/trajectoire.o: ../asservissement/trajectoire.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1141334063" 
	@${RM} ${OBJECTDIR}/_ext/1141334063/trajectoire.o.d 
	@${RM} ${OBJECTDIR}/_ext/1141334063/trajectoire.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../asservissement/trajectoire.c  -o ${OBJECTDIR}/_ext/1141334063/trajectoire.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1141334063/trajectoire.o.d"        -g -omf=elf -DPIC_BUILD=1 -DGROS=1 -std=c99 -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1141334063/trajectoire.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1299978293/protocole.o: ../common_code/uart/protocole.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1299978293" 
	@${RM} ${OBJECTDIR}/_ext/1299978293/protocole.o.d 
	@${RM} ${OBJECTDIR}/_ext/1299978293/protocole.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../common_code/uart/protocole.c  -o ${OBJECTDIR}/_ext/1299978293/protocole.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1299978293/protocole.o.d"        -g -omf=elf -DPIC_BUILD=1 -DGROS=1 -std=c99 -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1299978293/protocole.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1299978293/reception.o: ../common_code/uart/reception.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1299978293" 
	@${RM} ${OBJECTDIR}/_ext/1299978293/reception.o.d 
	@${RM} ${OBJECTDIR}/_ext/1299978293/reception.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../common_code/uart/reception.c  -o ${OBJECTDIR}/_ext/1299978293/reception.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1299978293/reception.o.d"        -g -omf=elf -DPIC_BUILD=1 -DGROS=1 -std=c99 -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1299978293/reception.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1299978293/text_reception.o: ../common_code/uart/text_reception.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1299978293" 
	@${RM} ${OBJECTDIR}/_ext/1299978293/text_reception.o.d 
	@${RM} ${OBJECTDIR}/_ext/1299978293/text_reception.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../common_code/uart/text_reception.c  -o ${OBJECTDIR}/_ext/1299978293/text_reception.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1299978293/text_reception.o.d"        -g -omf=elf -DPIC_BUILD=1 -DGROS=1 -std=c99 -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1299978293/text_reception.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/Asservissement.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/Asservissement.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -DPIC_BUILD=1 -DGROS=1 -std=c99  -mreserve=data@0x800:0x822   -Wl,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PICKIT2=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem$(MP_EXTRA_LD_POST) 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/Asservissement.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/Asservissement.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -DPIC_BUILD=1 -DGROS=1 -std=c99 -Wl,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem$(MP_EXTRA_LD_POST) 
	${MP_CC_DIR}\\xc16-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/Asservissement.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf  
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Robot
	${RM} -r dist/Robot

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif

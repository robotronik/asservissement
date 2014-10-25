OPTIONS=
POSTOPTIONS=
EXENAME=asser_robot
FICHIERS=main.c PID.c asser.c communication.c hardware.c meca.c odometrie.c trajectoire.c\
         PID.h   asser.h   communication.h   hardware.h   meca.h odometrie.h trajectoire.h\

.PHONY:all

all:$(EXENAME)

view:
	./$(EXENAME)

clean:
	rm -f *.o $(EXENAME) $(EXENAME).tar.bz2

tarall:$(EXENAME).tar.bz2

$(EXENAME).tar.bz2: $(FICHIERS)
	tar -jcvf $(EXENAME).tar.bz2 *

$(EXENAME): main.c asser.o PID.o communication.o hardware.o meca.o odometrie.o trajectoire.o
	gcc $(OPTIONS) main.c asser.o PID.o communication.o hardware.o meca.o odometrie.o trajectoire.o $(POSTOPTIONS) -o $(EXENAME)

asser.o: asser.c asser.h PID.h meca.h trajectoire.h odometrie.h
	gcc $(OPTIONS) -c asser.c 

PID.o:       PID.c  PID.h
	gcc $(OPTIONS) -c PID.c

communication.o:       communication.c  communication.h
	gcc $(OPTIONS) -c communication.c

hardware.o:       hardware.c  hardware.h
	gcc $(OPTIONS) -c hardware.c

meca.o:          meca.c     meca.h
	gcc $(OPTIONS) -c meca.c

odometrie.o:       odometrie.c  odometrie.h
	gcc $(OPTIONS) -c odometrie.c

trajectoire.o:       trajectoire.c  trajectoire.h
	gcc $(OPTIONS) -c trajectoire.c
OPTIONS=
POSTOPTIONS=
EXENAME=asser_robot
FICHIERS=main.c PID.c asser.c communication.c hardware.c meca.c odometrie.c trajectoire.c\
         PID.h   asser.h   communication.h   hardware.h   meca.h odometrie.h trajectoire.h\

.PHONY:view

view:all
	./$(EXENAME)

all:$(EXENAME)

clean:
	rm -f *.o *.h.gch $(EXENAME) $(EXENAME).tar.bz2

tarall:$(EXENAME).tar.bz2

$(EXENAME).tar.bz2: $(FICHIERS)
	tar -jcvf $(EXENAME).tar.bz2 *


$(EXENAME):         main.c asser.o
	gcc $(OPTIONS)  main.c asser.o $(POSTOPTIONS) -o $(EXENAME)

asser.o:       asser.c  PID.o  meca.o meca.o trajectoire.o odometrie.o asser.h
	gcc $(OPTIONS) asser.c PID.o  meca.o meca.o trajectoire.o odometrie.o

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
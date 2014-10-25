# Changez ces deux premières lignes pour modifier le chemin de vos librairies.

# Le tilde (~/) n'est pas supporté, remplacez-le par $(HOME)

# Le chemin relatif (../) est supporté.

# Modifiez les lignes de dépendances de headers en fonction de vos inclusions,
# pour ne pas "oublier" des compilations nécessaires !

PFLTK=/home/salamandar/FLTK/lib/FltkLinux64
PFMOD=/home/salamandar/FLTK/lib/FModLinux64
FMODLIB=$(PFMOD)/api/lib/libfmodex.so

OPTIONS=-I$(PFMOD) -I$(PFLTK) -I$(PFLTK)/jpeg
POSTOPTIONS=$(PFLTK)/lib/libfltk.a $(PFLTK)/lib/libfltk_forms.a $(PFLTK)/lib/libfltk_gl.a $(PFLTK)/lib/libfltk_images.a \
            $(PFLTK)/lib/libfltk_jpeg.a $(PFLTK)/lib/libfltk_png.a $(FMODLIB) \
            -lX11 -lXext -ldl -lm

EXENAME=projet
FICHIERS=main.cpp u1-interface.cpp u2-dessin.cpp u3-callbacks.cpp u4-fonctions.cpp \
         main.h   u1-interface.h   u2-dessin.h   u3-callbacks.h   u4-fonctions.h   \
         Autres\ Fichers,\ Par\ exemple\ un\ .pdf

.PHONY:view

view:all
	./$(EXENAME)

all:$(EXENAME)

clean:
	rm -f *.o *.h.gch $(EXENAME) $(EXENAME).tar.bz2

tarall:$(EXENAME).tar.bz2

$(EXENAME).tar.bz2: $(FICHIERS)
	tar -jcvf $(EXENAME).tar.bz2 *


$(EXENAME):         main.cpp u1-interface.o u2-dessin.o u3-callbacks.o u4-fonctions.o main.h
	g++ $(OPTIONS)  main.cpp u1-interface.o u2-dessin.o u3-callbacks.o u4-fonctions.o $(POSTOPTIONS) -o $(EXENAME)


u1-interface.o:       u1-interface.cpp  u1-interface.h u2-dessin.h u3-callbacks.h                   main.h
	g++ $(OPTIONS) -c u1-interface.cpp

u2-dessin.o:          u2-dessin.cpp     u1-interface.h u2-dessin.h u3-callbacks.h   u4-fonctions.h  main.h
	g++ $(OPTIONS) -c u2-dessin.cpp

u3-callbacks.o:       u3-callbacks.cpp  u1-interface.h u2-dessin.h u3-callbacks.h   u4-fonctions.h
	g++ $(OPTIONS) -c u3-callbacks.cpp

u4-fonctions.o:       u4-fonctions.cpp  u1-interface.h                              u4-fonctions.h
	g++ $(OPTIONS) -c u4-fonctions.cpp

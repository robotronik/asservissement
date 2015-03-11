#include "asser.h"
#include "trajectoire.h"
#include "odometrie.h"
#include "hardware.h"
#include "tests_unitaires.h"
#include "debug/affichage.h"
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <ctype.h>

#include "../robotronik.uart/text_reception.h"

void * main_loop()
{
	if (init_sdl_screen() < 0)
		return NULL;
	start();
    quit_sdl_screen();
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
// Simualation de l'uart

char uart_getc()
{
    // Interractive way
    return getc(stdin);
}

////////////////////////////////////////////////////////////////////////////////


bool is_whitespace(char c)
{
    return (c == ' ') || (c == '\t');
}

// pour new_xy_absolu
#include "communication.h"

// pourrait être déclaré ailleurs c'est pour l'exemple que ces variables sont
// ici
static int x = 0, y = 0;
static bool exit = 0;

void uart_interrupt()
{
    extern int x,y;
    ////////////////////

    enum {
        // valeurs
        KEY_X,
        KEY_Y,
        // control
        KEY_EXIT,
        KEY_END,

        KEY_SIZE,

        VAL_SIZE = KEY_EXIT
    };

    static char *keys[KEY_SIZE];
    keys[KEY_X]     = "x=";
    keys[KEY_Y]     = "y=";
    keys[KEY_EXIT]  = "q";
    keys[KEY_END]   = "end";
    static bool found[KEY_SIZE] = {true, true, true, true};

    static struct search_key_t sk = {
        0,
        KEY_SIZE,
        keys,
        found
    };

    static enum {
        KEY,
        VALUE_INT,
        WAIT_END,
    } state = KEY;

    static int *vals[VAL_SIZE] = {&x, &y};
    static int index_val;

    ////////////////////

    char c = tolower(uart_getc());
    int ret;

    debug("\n");
    debug("\n");
    debug("charactère lu : %c\n", c);

    switch (state) {
        case KEY:
            // Lecture d'une clé
            debug("reception clé\n");

            if(is_whitespace(c)) {
                debug("espace ignoré dans la clé\n");
                break;
            }

            ret = search_key(c, &sk);
            if (ret >= KEY_SIZE) {
                debug("ERREUR, clé inconnu: %d", ret);
            }
            else if (ret == -1) {
                debug("ERREUR, clé non trouvé: %d", ret);
                // erreur, on attend la fin du message courant
                state = WAIT_END;
            }
            else if (ret >= 0) {
                // On a fini de recevoir la clé
                debug("CLÉ TROUVÉ\n");

                switch (ret) {
                    case KEY_X:
                    case KEY_Y:
                        debug((ret == KEY_X)? "x\n" : "y\n");

                        // gestion des entiers
                        index_val = ret;
                        *vals[index_val] = 0;
                        state = VALUE_INT;
                        break;

                    case KEY_END:
                        // fin de trame
                        debug("fin de trame\n");
                        new_xy_absolu(x,y);
                        state = WAIT_END;
                        break;

                    case KEY_EXIT:
                        // fin du match
                        debug("FIN DE LA SIMULATION\n");
                        exit = true;
                        break;

                    default:
                        debug("ERREUR dans le programme de lecture des clées\n");
                }
            } else {
                // transmission en cours, on reste dans le même état
                debug("réception de la clé en cour");
            }
            break;

        case VALUE_INT:
            // Lecture d'un entier
            debug("lecture entier\n");

            if(is_whitespace(c)) {
                debug("espace ignoré durant la lecture\n");
                break;
            }

            debug("x = %d, y = %d\n", x, y);
            ret = read_int(c, vals[index_val]);
            debug("x = %d, y = %d\n", x, y);
            debug((index_val == KEY_X)? "x = " : "y = ");
            debug("%d\n", *vals[index_val]);

            if (ret == 0) {
                // la récéption n'est pas fini, on reste dans le même état
                debug("reception en cour\n");
            }
            else if (ret > 0) {
                // recetpion terminée
                debug("reception terminé\n");
                debug((index_val == KEY_X)? "x = ":"y = ");
                debug("%d\n", *vals[index_val]);

                // On se prépare à recevoir une nouvelle trame
                reset_search(&sk);
                state = KEY;
            }
            else if (ret == -1) {
                // erreur
                debug("ERREUR, d'overflow\n");
                *vals[index_val] = 0;
                state = WAIT_END;
            }
            else if (ret == -2) {
                debug("ERREUR, %c n'est pas un nombre\n", c);
                *vals[index_val] = 0;
                state = WAIT_END;
            }
            else {
                debug("ERREUR inconnu lors de la lecture d'une valeure : %d\n", ret);
                *vals[index_val] = 0;
                state = WAIT_END;
            }
            break;

        case WAIT_END:
            // On attend la fin de la trame
            debug("wait end\n");

            if (is_end(c)) {
                // On se prépare à recevoir une nouvelle trame
                reset_search(&sk);
                state = KEY;
            }
            break;
    }

    debug("etat final :\n");
    switch (state) {
        case KEY:       debug("KEY\n");       break;
        case VALUE_INT: debug("VALUE_INT\n"); break;
        case WAIT_END:  debug("WAIT_END\n");  break;
    }
}

////////////////////////////////////////////////////////////////////////////////

int main()
{

	
	/*init*/
	init_odometrie();
	init_trajectoire();
	init_hardware();
	init_asser();

	/*chemin pour le test*/
	s_liste chemin;
	chemin.taille=7;
	chemin.point[0].x=1500;
	chemin.point[0].y=1000;
	chemin.point[1].x=2650;
	chemin.point[1].y=300;
	chemin.point[2].x=2350;
	chemin.point[2].y=1200;
	chemin.point[3].x=2700;
	chemin.point[3].y=1600;
	chemin.point[4].x=2000;
	chemin.point[4].y=800;
	chemin.point[5].x=1000;
	chemin.point[5].y=1000;
	chemin.point[6].x=250;
	chemin.point[6].y=1000;
	//chemin.point[7].x=140; //position initiale du robot
	//chemin.point[7].y=140; //position initiale du robot

	/*tests pour réglage des parametres*/

		/*tests en boucle ouverte*/
	    //test_vitesse(MIN_VITESSE);
	    //test_ecretage();
	    //test_distance(500,200000);
	    //test_angle(3142,200000);

	    /*test d'asservissement*/
	    //test_asser_alpha_delta(0,400);
	    //test_asser_alpha_delta(3140,0);
	    //test_asser_theta(3142);
	    //test_asser_xy_relatif(0,400);
	    //test_asser_xy_absolu(140,400+140);
	    //test_asser_chemin(chemin);

    /*démarage de l'asservissement*/
	//start();

	/*évite un reset automatique du microcontroleur*/
    //while (1) {;}
    pthread_t thread_asser;
    int ret;

    ret = pthread_create (&thread_asser, NULL, main_loop, NULL);
    if (ret != 0)
        fprintf(stderr, "erreur %d\n", ret);

    //////////

    while(!exit) {
        uart_interrupt();
    }

    //////////

    ret = pthread_cancel(thread_asser);
    if (ret != 0)
        fprintf(stderr, "erreur %d\n", ret);
}

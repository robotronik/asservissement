#include <stdbool.h>
#include <ctype.h>

#include "../robotronik.uart/text_reception.h"
#include "reception.h"
#include "match.h"

// pour new_xy_absolu
#include "communication.h"

////////////////////////////////////////////////////////////////////////////////

static bool is_whitespace(char c);

// Contient la prochaine valeure de x et de y
static int x = 0, y = 0;

////////////////////////////////////////////////////////////////////////////////

static bool is_whitespace(char c)
{
    return (c == ' ') || (c == '\t');
}

void uart_interrupt(char uart_char)
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
    keys[KEY_END]   = "go";
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
        WAIT_GO,
    } state = KEY;

    static int *vals[VAL_SIZE] = {&x, &y};
    static int index_val;

    ////////////////////

    char c = tolower(uart_char);
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
                debug("ERREUR, clé inconnu: %d\n", ret);
            }
            else if (ret == -1) {
                debug("ERREUR, clé non trouvé: %d\n", ret);
                // erreur, on attend la fin du message courant
                if (is_end(c)) {
                    state = KEY;
                } else {
                    state = WAIT_GO;
                }
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
                        state = WAIT_GO;
                        break;

                    case KEY_EXIT:
                        // fin du match
                        debug("FIN DU MATCH\n");
                        match_set_etat(MATCH_FIN);
                        break;

                    default:
                        debug("ERREUR dans le programme de lecture des clées\n");
                }
            } else {
                // transmission en cours, on reste dans le même état
                debug("réception de la clé en cour\n");
            }
            break;

        case VALUE_INT:
            // Lecture d'un entier
            debug("lecture entier\n");

            if(is_whitespace(c)) {
                debug("espace ignoré durant la lecture\n");
                break;
            }

            ret = read_int(c, vals[index_val]);
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
                state = WAIT_GO;
            }
            else if (ret == -2) {
                debug("ERREUR, %c n'est pas un nombre\n", c);
                state = WAIT_GO;
            }
            else {
                debug("ERREUR inconnu lors de la lecture d'une valeure : %d\n", ret);
                *vals[index_val] = 0;
                state = WAIT_GO;
            }
            break;

        case WAIT_GO:
            // On attend la fin de la trame
            debug("wait end of trame\n");

            if (is_end(c)) {
                // On se prépare à recevoir une nouvelle trame
                reset_search(&sk);
                state = KEY;
            }
            break;
    }

    debug("etat final : ");
    switch (state) {
        case KEY:       debug("KEY\n");       break;
        case VALUE_INT: debug("VALUE_INT\n"); break;
        case WAIT_GO:   debug("WAIT_GO\n");  break;
    }
}

#include <stdbool.h>
#include <ctype.h>

#include "../common_code/common.h"
#include "../uart/text_reception.h"
#include "reception.h"
#include "match.h"

// pour new_xy_absolu
#include "communication.h"

////////////////////////////////////////////////////////////////////////////////

enum state_t {
    WAIT_KEY,
    WAIT_X,
    WAIT_Y,
    WAIT_GO,
};

static bool is_whitespace(char c);
enum state_t lecture_cle(char c, struct search_key_t *sk, int *x, int *y,
                         enum state_t current_state);
enum state_t lecture_val(char c, struct search_key_t *sk, int *val,
                         enum state_t current_state);
static enum state_t wait_end_of_trame(char c, struct search_key_t *sk,
    enum state_t current_state);

enum key_t {
    // valeurs
    KEY_X,
    KEY_Y,
    // control
    KEY_EXIT,
    KEY_END,

    // info
    KEY_SIZE,
    VAL_SIZE = KEY_EXIT
};

static char *keys[KEY_SIZE] = {
    [KEY_X]     = "x=",
    [KEY_Y]     = "y=",
    [KEY_EXIT]  = "q",
    [KEY_END]   = "go"
};

////////////////////////////////////////////////////////////////////////////////

static bool is_whitespace(char c)
{
    return (c == ' ') || (c == '\t');
}

enum state_t lecture_cle(char c, struct search_key_t *sk, int *x, int *y,
                         enum state_t current_state)
{
    // Lecture d'une clé
    debug("reception clé\n");

    if(is_whitespace(c)) {
        debug("espace ignoré dans la clé\n");
        return current_state;
    }

    int ret = search_key(c, sk);

    if (ret >= KEY_SIZE) {
        debug("ERREUR, clé inconnu: %d\n", ret);
    }

    // erreur, on attend la fin du message courant
    if (ret == -1) {
        debug("ERREUR, clé non trouvé: %d\n", ret);
        if (is_end(c)) {
            return WAIT_KEY;
        } else {
            return WAIT_GO;
        }
    }

    // On a fini de recevoir la clé
    if (ret >= 0) {
        debug("CLÉ TROUVÉ : ");
        debug((ret == KEY_X)? "x\n" : "y\n");

        switch (ret) {
            case KEY_X:
                return WAIT_X;
            case KEY_Y:
                return WAIT_Y;

            case KEY_END:
                // fin de trame
                debug("fin de trame\n");
                new_xy_absolu(*x,*y);
                return WAIT_GO;

            case KEY_EXIT:
                // fin du match
                // NB: ce cas n'est normalement pas à être géré par l'assert
                debug("FIN DU MATCH\n");
                match_set_etat(MATCH_FIN);
                return current_state;

            default:
                debug("ERREUR dans le programme de lecture des clées\n");
                return current_state;
        }
    }

    // transmission en cours, on reste dans le même état
    debug("réception de la clé en cour\n");
    return current_state;
}

enum state_t lecture_val(char c, struct search_key_t *sk, int *val,
                         enum state_t current_state)
{
    // Lecture d'un entier
    debug("lecture entier\n");

    if(is_whitespace(c)) {
        debug("espace ignoré durant la lecture\n");
        return current_state;
    }

    int ret = read_int(c, val);
    debug("value read: %d", *val);

    // la récéption n'est pas fini, on reste dans le même état
    if (ret == 0) {
        debug("reception en cour\n");
        return current_state;
    }

    // reception terminée
    if (ret > 0) {
        debug("reception terminé\n");
        debug("valeur: %d\n", *val);

        // On se prépare à recevoir une nouvelle trame
        reset_search(sk);
        return WAIT_KEY;
    }

    // erreur
    if (ret == -1) {
        debug("ERREUR, d'overflow\n");
        *val = 0;
        return WAIT_GO;
    }
    else if (ret == -2) {
        debug("ERREUR, %c n'est pas un nombre\n", c);
        return WAIT_GO;
    }
    else {
        debug("ERREUR inconnu lors de la lecture d'une valeure : %d\n", ret);
        *val = 0;
        return WAIT_GO;
    }
}

enum state_t wait_end_of_trame(char c, struct search_key_t *sk, enum state_t current_state)
{
    // On attend la fin de la trame
    debug("wait end of trame\n");

    if (is_end(c)) {
        // On se prépare à recevoir une nouvelle trame
        reset_search(sk);
        return WAIT_KEY;
    }

    return current_state;
}

void uart_interrupt(char uart_char)
{
    // Contient la prochaine valeure de x et de y
    static int x = 0, y = 0;

    static bool to_search[KEY_SIZE] = {true, true, true, true};

    static struct search_key_t sk = {
        0,
        KEY_SIZE,
        keys,
        to_search
    };

    static enum state_t state = WAIT_KEY;

    ////////////////////

    // on ne tient pas compte de la casse
    char c = tolower(uart_char);

    debug("\n");
    debug("\n");
    debug("charactère lu : %c\n", c);

    // on calcule l'état suivant
    switch (state) {
        case WAIT_KEY:
            state = lecture_cle(c, &sk, &x, &y, state);
            break;

        case WAIT_X:
            state = lecture_val(c, &sk, &x, state);
            break;
        case WAIT_Y:
            state = lecture_val(c, &sk, &y, state);
            break;

        case WAIT_GO:
            state = wait_end_of_trame(c, &sk, state);
            break;
    }

#if DEBUG
    debug("etat final : ");
    switch (state) {
        case WAIT_KEY:       debug("WAIT_KEY\n");       break;
        case WAIT_X: debug("WAIT_X\n"); break;
        case WAIT_Y: debug("WAIT_Y\n"); break;
        case WAIT_GO:   debug("WAIT_GO\n");  break;
    }
#endif
}

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
    WAIT_X,
    WAIT_Y,
    WAIT_ALPHA,
    WAIT_DELTA,
    WAIT_THETA,
    WAIT_FONCTION,
    WAIT_NEW_LINE,
};

static bool is_whitespace(char c);
enum state_t lecture_cle(char c, struct search_key_t *sk,
                         int *x, int *y, int *alpha, int *delta, int *theta,
                         enum state_t current_state);
enum state_t lecture_val(char c, struct search_key_t *sk, int *val,
                         enum state_t current_state);
static enum state_t wait_end_of_trame(char c, struct search_key_t *sk,
    enum state_t current_state);

enum key_t {
    // valeurs
    KEY_X,
    KEY_Y,
    KEY_ALPHA,
    KEY_DELTA,
    KEY_THETA,
    // control
    FCT_QUIT,
    FCT_ALPHA_DELTA,
    FCT_XY_RELATIF,
    FCT_XY_ABSOLU,
    FCT_THETA,

    // info
    KEY_SIZE,
    VAL_SIZE = KEY_THETA + 1
};

static char *keys[KEY_SIZE] = {
    [KEY_X]            = "x=",
    [KEY_Y]            = "y=",
    [KEY_ALPHA]        = "alpha=",
    [KEY_DELTA]        = "delta=",
    [KEY_THETA]        = "theta=",

    [FCT_QUIT]         = "q",
    [FCT_ALPHA_DELTA]  = "alpha_delta()",
    [FCT_XY_RELATIF]   = "xy_relatif()",
    [FCT_XY_ABSOLU]    = "xy_absolu()",
    [FCT_THETA]        = "theta()",
};

////////////////////////////////////////////////////////////////////////////////

static bool is_whitespace(char c)
{
    return (c == ' ') || (c == '\t');
}

enum state_t lecture_cle(char c, struct search_key_t *sk,
                         int *x, int *y, int *alpha, int *delta, int *theta,
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
            return WAIT_FONCTION;
        } else {
            return WAIT_NEW_LINE;
        }
    }

    // On a fini de recevoir la clé
    if (ret >= 0) {
        debug("CLÉ TROUVÉ : %s", keys[ret]);

        switch (ret) {
            case KEY_X:
                *x = 0;
                return WAIT_X;
            case KEY_Y:
                *y = 0;
                return WAIT_Y;
            case KEY_ALPHA:
                *alpha = 0;
                return WAIT_ALPHA;
            case KEY_DELTA:
                *delta = 0;
                return WAIT_DELTA;
            case KEY_THETA:
                *theta = 0;
                return WAIT_THETA;

            case FCT_QUIT:
                // fin du match
                // NB: ce cas n'est normalement pas à être géré par l'assert
                debug("FIN DU MATCH\n");
                match_set_etat(MATCH_FIN);
                return current_state;

            case FCT_ALPHA_DELTA:
                debug("fct_alpha_delta");
                new_alpha_delta(*alpha, *delta);
                return WAIT_NEW_LINE;

            case FCT_XY_RELATIF:
                debug("fct_xy_relatif");
                new_xy_relatif(*x, *y);
                return WAIT_NEW_LINE;

            case FCT_XY_ABSOLU:
                debug("fct_xy_absolu");
                new_xy_absolu(*x,*y);
                return WAIT_NEW_LINE;

            case FCT_THETA:
                debug("fct_theta");
                new_theta(*theta);
                return WAIT_NEW_LINE;

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
        return WAIT_FONCTION;
    }

    // erreur
    if (ret == -1) {
        debug("ERREUR, d'overflow\n");
        *val = 0;
        return WAIT_NEW_LINE;
    }
    else if (ret == -2) {
        debug("ERREUR, %c n'est pas un nombre\n", c);
        return WAIT_NEW_LINE;
    }
    else {
        debug("ERREUR inconnu lors de la lecture d'une valeure : %d\n", ret);
        *val = 0;
        return WAIT_NEW_LINE;
    }
}

enum state_t wait_end_of_trame(char c, struct search_key_t *sk, enum state_t current_state)
{
    // On attend la fin de la trame
    debug("wait end of trame\n");

    if (is_end(c)) {
        // On se prépare à recevoir une nouvelle trame
        reset_search(sk);
        return WAIT_FONCTION;
    }

    return current_state;
}

void uart_interrupt(char uart_char)
{
    // Contient la prochaine valeure des variables du programme
    static int x = 0, y = 0, alpha = 0, delta = 0, theta = 0;

    static bool to_search[KEY_SIZE] = {true, true, true, true, true,
                                       true, true, true, true, true};

    static struct search_key_t sk = {
        0,
        KEY_SIZE,
        keys,
        to_search
    };

    static enum state_t state = WAIT_FONCTION;

    ////////////////////

    // on ne tient pas compte de la casse
    char c = tolower(uart_char);

    debug("\n");
    debug("\n");
    debug("charactère lu : %c\n", c);

    // on calcule l'état suivant
    switch (state) {
        case WAIT_FONCTION:
            state = lecture_cle(c, &sk, &x, &y, &alpha, &delta, &theta, state);
            break;

        case WAIT_X:
            state = lecture_val(c, &sk, &x, state);
            break;
        case WAIT_Y:
            state = lecture_val(c, &sk, &y, state);
            break;
        case WAIT_ALPHA:
            state = lecture_val(c, &sk, &alpha, state);
            break;
        case WAIT_DELTA:
            state = lecture_val(c, &sk, &delta, state);
            break;
        case WAIT_THETA:
            state = lecture_val(c, &sk, &theta, state);
            break;

        case WAIT_NEW_LINE:
            state = wait_end_of_trame(c, &sk, state);
            break;
    }

    debug("etat final : %s", keys[state]);
}

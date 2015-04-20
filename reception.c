#include <stdbool.h>
#include <ctype.h>

#include "../common_code/common.h"
#include "../uart/text_reception.h"
#include "reception.h"
#include "match.h"
#include "communication.h"
#include "odometrie.h"
#include "trajectoire.h"

////////////////////////////////////////////////////////////////////////////////

enum state_t {
    WAIT_X,
    WAIT_Y,
    WAIT_ALPHA,
    WAIT_DELTA,
    WAIT_THETA,
    WAIT_FONCTION,
    WAIT_NEW_LINE,
    STATE_SIZE,
};

#if DEBUG
static char *state_name[STATE_SIZE] = {
    [WAIT_X] = "wait_x",
    [WAIT_Y] = "wait_y",
    [WAIT_ALPHA] = "wait_alpha",
    [WAIT_DELTA] = "wait_delta",
    [WAIT_THETA] = "wait_theta",
    [WAIT_FONCTION] = "wait_fonction",
    [WAIT_NEW_LINE] = "wait_new_line",
};
#endif

static bool is_whitespace(char c);
enum state_t lecture_cle(char c, struct search_key_t *sk,
                         int *x, int *y, int *alpha, int *delta, int *theta, s_liste *chemin,
                         enum state_t current_state);
enum state_t lecture_val(char c, struct search_key_t *sk, int *val,
                         enum state_t current_state);
static enum state_t wait_end_of_trame(char c, struct search_key_t *sk,
    enum state_t current_state);
void help();
void efface_chemin(s_liste *chemin);
void add_point(int x, int y, s_liste *chemin);

enum key_t {
    // valeurs
    KEY_X,
    KEY_Y,
    KEY_ALPHA,
    KEY_DELTA,
    KEY_THETA,
    // commandes
    CMD_QUIT,
    CMD_HELP,
    // fonction
    FCT_ALPHA_DELTA,
    FCT_XY_RELATIF,
    FCT_XY_ABSOLU,
    FCT_THETA,
    FCT_ADD,
    FCT_CLEAR,
    FCT_CHEMIN,
    FCT_UPDATE,
    FCT_MODE_TENDU,
    FCT_MODE_COURBE,

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

    [CMD_QUIT]         = "q",
    [CMD_HELP]         = "?",

    [FCT_ALPHA_DELTA]  = "alpha_delta()",
    [FCT_XY_RELATIF]   = "xy_relatif()",
    [FCT_XY_ABSOLU]    = "xy_absolu()",
    [FCT_THETA]        = "theta()",
    [FCT_ADD]          = "add()",
    [FCT_CHEMIN]       = "chemin()",
    [FCT_CLEAR]        = "clear()",
    [FCT_UPDATE]       = "update()",
    [FCT_MODE_TENDU]   = "mode(tendu)",
    [FCT_MODE_COURBE]  = "mode(courbe)",
};

#if DEBUG
static char *keys_help[KEY_SIZE] = {
    [KEY_X]            = "coordonnées absolu en cm",
    [KEY_Y]            = "coordonnées absolu en cm",
    [KEY_ALPHA]        = "angle relatif en ???? degré? milidegré?",
    [KEY_DELTA]        = "distance relative en cm",
    [KEY_THETA]        = "angle absolu en ???? degré? milidegré?",

    [CMD_QUIT]         = "quitter la simulation",
    [CMD_HELP]         = "affiche l'aide",

    [FCT_ALPHA_DELTA]  = "new_alpha_delta(alpha, delta)",
    [FCT_XY_RELATIF]   = "new_xy_relatif(x,y)",
    [FCT_XY_ABSOLU]    = "new_xy_absolu(x,y)",
    [FCT_THETA]        = "new_theta(theta)",
    [FCT_ADD]          = "Ajoute les points x et y dans le prochain chemin",
    [FCT_CLEAR]        = "Efface le chemin en cours de construction",
    [FCT_CHEMIN]       = "Envoie le chemin précédemment construit",
    [FCT_UPDATE]       = "met à jour les variables du protocole de simulation "
            "pour qu'elle correspondent à celle utilisées par l'assert",
    [FCT_MODE_TENDU]   = "déplacement en mode tendu",
    [FCT_MODE_COURBE]  = "déplacement en mode courbe",
};
#endif

////////////////////////////////////////////////////////////////////////////////

static bool is_whitespace(char c)
{
    return (c == ' ') || (c == '\t');
}

enum state_t lecture_cle(char c, struct search_key_t *sk,
                         int *x, int *y, int *alpha, int *delta, int *theta, s_liste *chemin,
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
        debug("ERREUR, clé non trouvé\n");
        help();
        if (is_end(c)) {
            return WAIT_FONCTION;
        } else {
            return WAIT_NEW_LINE;
        }
    }

    // On a fini de recevoir la clé
    if (ret >= 0) {
        debug("CLÉ TROUVÉ : %s\n", keys[ret]);

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

            case CMD_QUIT:
                // fin du match
                // NB: ce cas n'est normalement pas à être géré par l'assert
                debug("FIN DU MATCH\n");
                match_set_etat(MATCH_FIN);
                return current_state;

            case CMD_HELP:
                help();
                return WAIT_NEW_LINE;

            case FCT_ALPHA_DELTA:
                debug("new_alpha_delta(%d, %d);\n", *alpha, *delta);
                new_alpha_delta(*alpha, *delta);
                return WAIT_NEW_LINE;

            case FCT_XY_RELATIF:
                debug("new_xy_relatif(%d, %d);\n", *x, *y);
                new_xy_relatif(*x, *y);
                return WAIT_NEW_LINE;

            case FCT_XY_ABSOLU:
                debug("new_xy_absolu(%d, %d);\n", *x,*y);
                new_xy_absolu(*x,*y);
                return WAIT_NEW_LINE;

            case FCT_THETA:
                debug("new_theta(%d);\n", *theta);
                new_theta(*theta);
                return WAIT_NEW_LINE;

            case FCT_ADD:
                add_point(*x, *y, chemin);
                return WAIT_NEW_LINE;

            case FCT_CLEAR:
                efface_chemin(chemin);
                return WAIT_NEW_LINE;

            case FCT_CHEMIN:
                add_point(*x, *y, chemin);
#if DEBUG
                debug("Envoie du chemin\n");
                for (int i = 0; i < chemin->taille; i++) {
                    debug("	%d, %d\n", chemin->point[i].x, chemin->point[i].y);
                }
#endif
                set_trajectoire_chemin(*chemin);
                efface_chemin(chemin);
                return WAIT_NEW_LINE;

            case FCT_UPDATE:
                debug("update()\n");
                *theta = get_theta_actuel();
                *x     = get_x_actuel();
                *y     = get_y_actuel();
                *alpha = get_alpha_actuel();
                *delta = get_delta_actuel();
                debug("Desormais, x = %d, y=%d, theta=%d, alpha=%d, delta=%d\n", *x, *y, *theta, *alpha, *delta);
                return WAIT_NEW_LINE;

            case FCT_MODE_TENDU:
                debug("utilisation du mode tendu\n");
                set_trajectoire_mode(tendu);
                return WAIT_NEW_LINE;

            case FCT_MODE_COURBE:
                set_trajectoire_mode(courbe);
                debug("utilisation du mode courbe\n");
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
    debug("value read: %d\n", *val);

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
    static s_liste chemin = { .taille = 0 };

    static bool to_search[KEY_SIZE] = {true, true, true, true, true,
                                       true, true, true, true, true,
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
            state = lecture_cle(c, &sk, &x, &y, &alpha, &delta, &theta, &chemin, state);
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

        case STATE_SIZE: // pour éviter un warning inutile
            break;
    }

    debug("etat final : %s\n", state_name[state]);
}


void help()
{
    /*printf("\n-------------------------------\n");
    printf("Liste des commandes supportées:\n");
    for (int i = 0; i < KEY_SIZE; i++) {
        printf("%s", keys[i]);
#if DEBUG
        printf("		%s", keys_help[i]);
#endif
        printf("\n");
    }
    printf("-------------------------------\n\n");*/
}

void efface_chemin(s_liste *chemin)
{
    debug("chemin effacé\n");
    chemin->taille = 0;
}

void add_point(int x, int y, s_liste *chemin)
{
    debug("ajout du point %d, %d\n", x, y);
    if (chemin->taille >= MAX_POSITIONS) {
        debug("\nAttention, le chemin est **trop long**. Point ignoré\n");
        return;
    }
    chemin->point[chemin->taille].x = x;
    chemin->point[chemin->taille].y = y;
    chemin->taille++;
}

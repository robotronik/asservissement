#include <stdbool.h>

#include "match.h"

// mémorise l'état courant du match
static enum Match_etat s_match = MATCH_PREPARATION;

////////////////////////////////////////////////////////////////////////////////

enum Match_etat match_get_etat()
{
    return s_match;
}

void match_set_etat(enum Match_etat new_state)
{
    // TODO: ajouter du debug(pour vérifier que l'on est bien en train de faire
    // avancer le match

    // TODO: ajouter un appel à match_set_etat(MATCH_FIN) lorsque l'on ferme la fenetre SDL

    s_match = new_state;
}

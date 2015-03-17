#ifndef MATCH_H
#define MATCH_H

enum Match_etat {
    MATCH_PREPARATION,
    MATCH_EN_COUR,
    MATCH_FIN
};

/** Retourne l'état courant du match
 *
 * \note À l'intialisation, l'état est MATCH_PREPARATION
 */
enum Match_etat match_get_etat();

/** Met à jour l'état du match
 *
 * Le nouvel état doit être dans l'ordre chronologique
 *  MATCH_PREPARATION -> MATCH_EN_COUR -> MATCH_FIN
 */
void match_set_etat(enum Match_etat new_state);

#endif /* MATCH_H */

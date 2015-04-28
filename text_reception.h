#ifndef TEXT_RECEPTION_H
#define TEXT_RECEPTION_H

#include <stdbool.h>

/* ======================================= */
/* PROTOCOLE DE TRANSMISSION ORIENTÉ TEXTE */
/* ======================================= */

/* UTILISATION
 *
 * En émission les fonctions standard d'émission de texte sont utilisables
 * (printf entre autre). Par conséquent, il est très simple de vérifier que les
 * messages sont correctement transmis en utilisant les outils standards UNIX.
 *
 * En lecture, des primitives sont fournies dans le fichier ci-dessous pour
 * faciliter le découpage du texte.
 *
 * La gestion des espaces blancs (espace, tabs, …) est laissée à l'utilisateur.
 */

/* Il est conseillé d'utiliser le schéma suivant:
 *
 * clé1=valeur1
 * clé2=valeur2
 * validation
 *
 * La gestion en ligne fait qu'il est très simple de s'assurer que la réception
 * est prête à recevoir un message : il suffit d'envoyer un retour chariot.
 */

struct search_key_t {
    int index;             ///< index du prochain caractère à lire
    int nb_keys;           ///< taille de keys[] en nombre d'élément
    char **keys;           ///< tableaux contenant les clés à rechercher
    bool *to_search;       ///< il faut chercher keys[i] si to_search[i] == true
};

/** prépare [sk] pour une nouvelle recherche
 */
void reset_search(struct search_key_t *sk);

/** Cherche si [c] est une clé valide dans [sk]
 *
 * Le caractère [c] va être comparé au caractère à l'indice [sk.index] des
 * chaines [sk.keys], à condition que la chaine soit à rechercher (ce qui
 * équivaut à [sk.keys.to_search == true).
 *
 * Si [c] est un caractère de fin de séquence (\0, \n et \r), alors l'indice de
 * la première chaine à rechercher est retourné.
 *
 * Sinon, [keys.to_search] va être mis à jour, et -(le nombre de chaine encore
 * valide + 1) est retourné. Par conséquent, -1 signifie qu'il n'y a plus
 * aucune chaine valide.
 */
int search_key(char c, struct search_key_t *sk);

/** Lit une chaine de caractère
 * Aucun '\0' final n'est ajouté
 * index est automatiquement incrémenté.
 * return -1 en cas de buffer overflow (et *index = size_str)
 * return 0 si la réception n'est pas terminée
 * return index (= la taille de la chaine de charactères) si la chaine est complete
 */
int read_string(char c, int *index, char *str, int size_str);

/** Lit un entier
 * return 0 tant que la réception n'est pas terminée
 * return >0 quand la reception est terminée
 * return -1 en cas d'overflow
 * return -2 si l'entrée n'est pas un nombre
 */
int read_int(char c, int *val);

/** Return true si [c] est un caractère de find de ligne ou de fichier (\0, \r
 * et \n)
 */
bool is_end(char c);

#endif /* TEXT_RECEPTION_H */

#include "trajectoire.h"
/* ================================= */
/* PROTOCOLE DE TRANSMISSION TEXTE   */
/* ================================= */

// Ici devra exister un gros "switch" traitant toutes les clés ( = define)
// pour appeler les fonctions qu'il faut dans l'asser.

// Les fonctions commentées ci-dessous listent l'ensemble des événements à gérer.





/*
int received_x = 0,
    received_y = 0;
int received_alpha = 0,
    received_delta = 0,
    received_theta = 0;

// Réception de coordonnées
void uart_received_coordonnees(int x, int y) {
    received_x = x;
    received_y = y;
}
void uart_received_alphadelta(int alpha, int delta) {
    received_alpha = alpha;
    received_delta = delta;
}
void uart_received_theta(int theta) {
    received_theta = theta;
}

// Gestion des chemins de points
void uart_received_nouveau_point_dans_chemin() {
    if(!add_point_chemin(received_x, received_y)) {
        debug(1, "\nAttention, le chemin est **trop long**. Point ignoré\n");
    }
}

void uart_received_execute_le_chemin() {
    //set_trajectoire_chemin();
}

// Déplacements classiques
void uart_received_mouvement_xy_absolu() {
    set_trajectoire_xy_absolu(received_x, received_y);
}
void uart_received_mouvement_xy_relatif() {
    set_trajectoire_xy_relatif(received_x, received_y);
}

// Tour sur soi-même, par exemple
void uart_received_mouvement_alphadelta() {
    set_trajectoire_alpha_delta(received_alpha, received_delta);
}
void uart_received_mouvement_alpha() {
    set_trajectoire_alpha_delta(received_alpha, 0);
}
void uart_received_mouvement_delta() {
    set_trajectoire_alpha_delta(0, received_delta);
}
void uart_received_mouvement_theta() {
    set_trajectoire_alpha_theta(received_theta);
}

// Modes de déplacement
void uart_received_mode_tendu() {
    set_trajectoire_mode(tendu);
}
void uart_received_mode_courbe() {
    set_trajectoire_mode(courbe);
}
void uart_received_stop_normal() {
}
void uart_received_stop_urgence() {
}


*/

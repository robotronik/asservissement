
/* ================================= */
/* PROTOCOLE DE TRANSMISSION TEXTE   */
/* ================================= */


//*************** Implémentation de l'API de réception :
/* Voici les définitions des fonctions déclarées dans l'UART.
 * On peut bien sûr implémenter des fonctions vides, si on ne veut pas
 * gérer certains messages ! ;)

 (Quoi, moi, influencé par le java ? Noooooooon…)
 */

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
}
void uart_received_execute_le_chemin() {
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
}
void uart_received_mode_courbe() {
}
void uart_received_stop_normal() {
}
void uart_received_stop_urgence() {
}


// "Feedback" de l'asser à la stratégie
void uart_received_position_atteinte() {
}


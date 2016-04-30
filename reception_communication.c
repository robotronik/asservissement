// Fonctions de l'asservissement

#include "reception_communication.h"
#include <debug.h>

#include "odometrie.h"
#include "trajectoire.h"
#include "PID.h"

Reception_object values;

void init_reception_communication(){
    init_reception(&values, callbacks);
}

void reception_communication(char c) {
    lecture_message((char) c, &values);
}

char buffer[40];

int received_alpha,
    received_delta,
    received_theta,
    received_x,
    received_y;

s_liste received_chemin = { .taille = 0 };


int kp_delta,
    kd_delta,
    kp_alpha,
    kd_alpha;

int reception_set_x() {
    received_x = values.received_value;
    return 0;
}
int reception_set_y() {
    received_y = values.received_value;
    return 0;
}
int reception_set_alpha() {
    received_alpha = values.received_value;
    return 0;
}
int reception_set_delta() {
    received_delta = values.received_value;
    return 0;
}
int reception_set_theta() {
    received_theta = values.received_value;
    return 0;
}

int reception_set_KPd() {
    kp_delta = values.received_value;
    return 0;
}
int reception_set_KDd() {
    kd_delta = values.received_value;
    return 0;
}
int reception_set_KPa() {
    kp_alpha = values.received_value;
    return 0;
}
int reception_set_KDa() {
    kd_alpha = values.received_value;
    return 0;
}

int reception_alpha_delta() {
    set_trajectoire_alpha_delta(received_alpha, received_delta);
    return 0;
}
int reception_xy_relatif() {
    set_trajectoire_xy_relatif(received_x, received_y);
    return 0;
}
int reception_xy_absolu() {
    set_trajectoire_xy_absolu(received_x, received_y);
    return 0;
}
int reception_theta() {
    set_trajectoire_theta(received_theta);
    return 0;
}


// Chemins
int reception_add() {
    debug(_DEBUG_, "ajout du point %d, %d\n", received_x, received_y);
    if (received_chemin.taille >= MAX_POSITIONS) {
        debug(_ERROR_, "\nAttention, le chemin est **trop long**. Point ignoré\n");
        return 0;
    }
    received_chemin.point[received_chemin.taille].x = received_x;
    received_chemin.point[received_chemin.taille].y = received_y;
    received_chemin.taille++;
    return 0;
}
int reception_clear() {
    debug(_DEBUG_, "chemin effacé\n");
    received_chemin.taille = 0;
    return 0;
}
int reception_chemin() {
    reception_add();
    debug(_DEBUG_, "Envoi du chemin\n");
    for (int i = 0; i < received_chemin.taille; i++) {
        debug(_DEBUG_, "\t%d, %d\n", received_chemin.point[i].x, received_chemin.point[i].y);
    }
    set_trajectoire_chemin(received_chemin);
    reception_clear();
    return 0;
}

int reception_mode_tendu() {
    debug(_DEBUG_, "utilisation du mode tendu\n");
    set_trajectoire_mode(tendu);

    return 0;
}
int reception_mode_courbe() {
    debug(_DEBUG_, "utilisation du mode courbe\n");
    set_trajectoire_mode(courbe);
    return 0;
}

int reception_set_pos() {
    set_x_actuel(received_x);
    set_y_actuel(received_y);
    set_theta_actuel(received_theta);
    return 0;
}
int reception_change_pid() {
    set_kp_delta(kp_delta);
    set_kd_delta(kd_delta);
    set_kp_alpha(kp_alpha);
    set_kd_alpha(kd_alpha);

    return 0;
}


int reception_done() {
    return 0;
}
int reception_get_pos() {
    send_val(buffer, keys[VAL_X],get_x_actuel());
    UART_send_message(buffer, 40);
    send_val(buffer, keys[VAL_Y],get_y_actuel());
    UART_send_message(buffer, 40);
    send_val(buffer, keys[VAL_THETA],get_theta_actuel());
    UART_send_message(buffer, 40);
    send_cmd(buffer, keys[CMD_SEND_POS]);
    UART_send_message(buffer, 40);

    return 0;
}

int reception_quit() {
    return 0;
}


int reception_estop() {
    set_trajectoire_emergency_stop();
    return 0;
}

int reception_stop() {
    set_trajectoire_stop();
    return 0;
}



callback_t callbacks[KEYS_SIZE] = {
    //// Ces fonctions attendent une valeur entière
    [VAL_X]             = reception_set_x,
    [VAL_Y]             = reception_set_y,
    [VAL_ALPHA]         = reception_set_alpha,
    [VAL_DELTA]         = reception_set_delta,
    [VAL_THETA]         = reception_set_theta,

    [VAL_KP_DELTA]      = reception_set_KPd,
    [VAL_KD_DELTA]      = reception_set_KDd,
    [VAL_KP_ALPHA]      = reception_set_KPa,
    [VAL_KD_ALPHA]      = reception_set_KDa,

    // Avant, on a un arg. Après, pas d'args.

    //// Ces commandes ne demandent pas d'argument

    // Fonctions : à appeler après avoir envoyé des valeurs pour "confirmer"
    [FCT_ALPHA_DELTA]   = reception_alpha_delta,
    [FCT_XY_RELATIF]    = reception_xy_relatif,
    [FCT_XY_ABSOLU]     = reception_xy_absolu,
    [FCT_THETA]         = reception_theta,

    [FCT_ADD]           = reception_add,
    [FCT_CLEAR]         = reception_clear,
    [FCT_CHEMIN]        = reception_chemin,

    [FCT_MODE_TENDU]    = reception_mode_tendu,
    [FCT_MODE_COURBE]   = reception_mode_courbe,

    [FCT_SET_POS]       = reception_set_pos,
    [FCT_SET_PID]       = reception_change_pid,


    [CMD_DONE]          = reception_done,
    [CMD_GET_POS]       = reception_get_pos,
    [CMD_SEND_POS]      = NULL,
    [CMD_QUIT]          = NULL, // TODO ?
    [CMD_HELP]          = NULL,

    [CMD_STOP]          = reception_stop,
    [CMD_EMERGENCY_STOP]= reception_estop,

};

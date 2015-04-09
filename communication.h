#ifndef COMMUNICATION_H
#define COMMUNICATION_H

void analyse_message();
void send_position_atteinte();
//void send_asser_pret(); ??

//appelées de l'extérieur uniquement pour les tests unitaires
void new_alpha_delta(int alpha, int delta);
void new_xy_relatif(int x, int y);
void new_xy_absolu(int x, int y);
void set_mode_tendu();
void set_mode_courbe();
void new_theta(int theta);
void new_chemin();

#endif

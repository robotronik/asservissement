#ifndef ODOMETRIE_H
#define ODOMETRIE_H

#define DEUX_PI 6.2832
#define PI 3.14159

void init_odometrie();
void actualise_position();

//appelées de l'extérieur uniquement pour les tests unitaires
long int delta_mm(long int nbr_tick_D, long int nbr_tick_G);
long int alpha_millirad(long int nbr_tick_D, long int nbr_tick_G);

//utile pour l'asser et les calculs de trajectoires et peut être demandé par la stratégie
long int get_delta_actuel();
long int get_alpha_actuel();
int get_theta_actuel();
int get_x_actuel();
int get_y_actuel();

#endif
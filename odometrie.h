#ifndef ODOMETRIE_H
#define ODOMETRIE_H

#define PI_MILLI 3142
#define DEUX_PI_MILLI 6283

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
float float_get_x_actuel();
float float_get_y_actuel();

void set_theta_actuel(int new_theta);
void set_x_actuel(int new_x);
void set_y_actuel(int new_y);

#endif

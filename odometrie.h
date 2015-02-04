#ifndef ODOMETRIE_H
#define ODOMETRIE_H

#define DEUX_PI 6.2832
#define PI 3.14159

void init_odometrie();

void actualise_position();

long int delta_mm(long int nbr_tick_D, long int nbr_tick_G);
long int alpha_millirad(long int nbr_tick_D, long int nbr_tick_G);
void actualise_xy(int d_delta, int d_alpha, int theta, float * x, float * y);
int borne_angle(long int angle);

long int get_delta_actuel();
long int get_alpha_actuel();
int get_theta_actuel();
int get_x_actuel();
int get_y_actuel();

#endif
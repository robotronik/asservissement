#ifndef ODOMETRIE_H
#define ODOMETRIE_H

#define DEUX_PI 6.2832

void init_odometrie();

void set_delta_actuel(int delta);
void set_alpha_actuel(int alpha);

int get_delta_actuel();
int get_alpha_actuel();
int get_theta_actuel();
int get_x_actuel();
int get_y_actuel();

void actualise_position();

#endif
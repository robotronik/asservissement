#ifndef TRAJECTOIRE_H
#define TRAJECTOIRE_H

void init_trajectoire();
void init_alpha_delta_voulu();

void set_delta_voulu(int delta);
void set_alpha_voulu(int alpha);
void set_x_voulu_absolu(int x);
void set_x_voulu_absolu(int x);

void consigne_type_is_xy(int a);

int get_delta_voulu();
int get_alpha_voulu();

void consigne_new_alpha_delta(int new_alpha, int new_delta);
void consigne_new_xy_relatif(int x_voulu, int y_voulu);
void consigne_new_xy_absolu(int x_voulu, int y_voulu);
void consigne_new_theta(int theta_voulu);

void update_consigne();

#endif
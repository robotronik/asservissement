#ifndef PID_H
#define PID_H

#include "asser.h"

long int PID_lineique(s_erreur erreur_delta);
long int PID_angulaire(s_erreur erreur_alpha);

void set_kp_delta(int new_kp_delta);
void set_kd_delta(int new_kd_delta);
void set_kp_alpha(int new_kp_alpha);
void set_kd_alpha(int new_kd_alpha);

#endif
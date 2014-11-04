#ifndef PID_H
#define PID_H

long int PID_lineique(int erreur, int erreur_preced, int erreur_sum);
long int PID_angulaire(int erreur, int erreur_preced, int erreur_sum);

#endif
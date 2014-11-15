#ifndef ASSER_H
#define ASSER_H

//macro pour le calcul de valeur absolue
#define abs(x) ((x) < 0 ? - (x) : (x))

typedef struct
{
	int preced;
	int actuelle;
	int sum;
} s_erreur;

void asser();
void update_erreurs(s_erreur * erreur_delta, s_erreur * erreur_alpha);
void ecretage_reponse(long int * reponse,long int reponse_preced);
int asser_done(int erreur_delta, int erreur_alpha);
int arret_ok(long int commande_moteur_D,long int commande_moteur_G);
int convert2PWM(long int commande);
//long int abs(long int entier_relatif);

#endif
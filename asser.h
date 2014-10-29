#define abs(x) ((x) < 0 ? - (x) : (x))

void asser();
void verifie_reponse(long int * reponse,long int reponse_preced);
int asser_done(int erreur_delta, int erreur_alpha);
int arret_ok(long int commande_moteur_D,long int commande_moteur_G);
int convert2PWM(long int commande);
//long int abs(long int entier_relatif);
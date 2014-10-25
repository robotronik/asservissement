#define PRECISION_DELTA 1 //precision souhaitée pour delta en mm
#define PRECISION_ALPHA 1 //precision souhaitée pour alpha en rad
#define VIT_MAX_ARRET 15000 //vitesse maximale à laquelle on peut s'arreter sans tomber

void asser();
void valide(int * reponse,int reponse_preced);
int convert2PWM(int commande);
#define ENTRAXE //mm
#define DEMI_ENTRAXE 5 //mm ENTRAXE divisé par 2
#define MAX_VITESSE 40000 //vitesse maximale du robot
#define MIN_VITESSE 8000 //vitesse minimale du robot pour qu'il commence à avancer
#define MAX_ACCELERATION 20 //acceleration maximale
#define MAX_DECELERATION 20 //deceleration minimale

void set_PWM_moteur_D(int PWM);
void set_PWM_moteur_G(int PWM);
int get_nbr_tick_D();
int get_nbr_tick_G();
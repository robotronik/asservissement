#define ENTRAXE //mm
#define DEMI_ENTRAXE ENTRAXE/2 //mm
#define MAX_VITESSE //vitesse maximale du robot
#define MIN_VITESSE //vitesse minimale du robot pour qu'il commence à avancer
#define MAX_ACCELERATION //acceleration maximale
#define MAX_DECELERATION //deceleration minimale

void set_PWM_moteur_D(int PWM);
void set_PWM_moteur_G(int PWM);
int get_nbr_tick_D();
int get_nbr_tick_G();
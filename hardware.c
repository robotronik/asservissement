/*//ici la couche d'abstraction du microcontroleur utilisé
//on fait ceci afin de rendre le code portable

void set_PWM_moteur_D(int PWM)
{}

void set_PWM_moteur_G(int PWM)
{}

int get_nbr_tick_D()
{}

int get_nbr_tick_G()
{}

int doit_attendre()
{
	//synchronisation de asser() à une fréquence régulière
}

//ajouter fonction d'envoi et de réception par l'uart
//une fois quelque chose reçu on envoie à "communicution.c"*/

#include <math.h>
#include "hardware.h"

int PWM_D;
int PWM_G;

void set_PWM_moteur_D(int PWM)
{
	PWM_D=PWM/10;
}

void set_PWM_moteur_G(int PWM)
{
	PWM_G=PWM/10;
}

int get_nbr_tick_D()
{
	return PWM_D*10;
}

int get_nbr_tick_G()
{
	return PWM_G*10;
}

int doit_attendre()
{
	return 0;
}

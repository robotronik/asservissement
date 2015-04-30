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
#include "../common_code/common.h"

long int PWM_D;
long int PWM_G;
int moteurs_arret=0;


void init_hardware()
{}

void set_PWM_moteur_D(int PWM)
{
	if (!moteurs_arret)
	{
		PWM_D+=PWM/10;
	}
}

void set_PWM_moteur_G(int PWM)
{
	if (!moteurs_arret)
	{
		PWM_G+=PWM/10;
	}
}

long int get_nbr_tick_D()
{
	return PWM_D*5;
}

long int get_nbr_tick_G()
{
	return PWM_G*5;
}

int attente_synchro()
{
	return 0;
}

void motors_stop()
{
	moteurs_arret=1;
}

void reset_synchro()
{}

void UART_send_message(char* message) {
    char *actuel = message;
    while (*actuel)
        debug_byte(0,  *actuel++);
    debug_byte(0,'\0');
}
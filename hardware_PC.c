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
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include "../hardware/debug.h"

#if USE_SDL
#	include "odometrie.h"
#endif

#define SYNCHRO_TIME 0.016

long int PWM_D;
long int PWM_G;
int moteurs_arret=0;
long preced_clock=0;

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

void attente_synchro()
{
    //boucle d'attente
    long actual_clock= (long) clock();
    while((double)(actual_clock-preced_clock)/CLOCKS_PER_SEC<SYNCHRO_TIME)
    {
		actual_clock= (long) clock();
    }
    preced_clock=actual_clock;
}

void motors_stop()
{
	moteurs_arret=1;
}



void allumer_del()
{

}

void eteindre_del()
{
	// It will call some GTK thing
}

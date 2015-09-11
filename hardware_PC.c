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

#define RX_BUFFER_SIZE 40
#define SYNCHRO_TIME 0.016

long int PWM_D;
long int PWM_G;
int moteurs_arret=0;
long preced_clock=0;
int cmd_quit_received=0;

static unsigned char rxBuffer[RX_BUFFER_SIZE];
static unsigned short rxBufferDebut=0;
static unsigned short rxBufferFin=0;

int arret()
{
	return cmd_quit_received;
}

void * fake_RX()
{
	while(!arret()) {
		// On lit l'entrée standard pour simuler une reception sur l'UART
		// Si on reçoit "q/n" on quitte le programme
		char c=getc(stdin);
		if(c=='q')
		{
			char d=getc(stdin);
			if (d=='\n' || d=='\r' || d=='\0')
			{
				cmd_quit_received=1;
			}
			else
			{
				rxBuffer[rxBufferFin] = c;
				rxBuffer[rxBufferFin+1] = d;
				rxBufferFin = (rxBufferFin + 2) % RX_BUFFER_SIZE;
			}
		}
		else
		{
			rxBuffer[rxBufferFin] = c;
			rxBufferFin = (rxBufferFin + 1) % RX_BUFFER_SIZE;
		}
	}
	return NULL;
}


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

int UART_getc(unsigned char *c)
{
	if (rxBufferDebut == rxBufferFin) {
		// Il n'y avait pas de caractères en attente
		return 0;
	} else {
		// Il y des caractères à traiter
		*c = rxBuffer[rxBufferDebut];
		rxBufferDebut = (rxBufferDebut + 1) % RX_BUFFER_SIZE;
		return 1;
	}
}

void UART_send_message(char* message) {
	char *actuel = message;
	while (*actuel)
		debug_byte(0,  *actuel++);
	debug_byte(0,'\0');
}

void allumer_del()
{

}

void eteindre_del()
{
	// It will call some GTK thing
}

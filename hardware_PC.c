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
#include <pthread.h>
#include "match.h"

#define RX_BUFFER_SIZE 40

long int PWM_D;
long int PWM_G;
int moteurs_arret=0;

static unsigned char rxBuffer[RX_BUFFER_SIZE];
static unsigned short rxBufferDebut=0;
static unsigned short rxBufferFin=0;

void * fake_RX()
{
	while(match_get_etat() != MATCH_FIN) {
		// On lit l'entrÃ©e standard, et on passe les caractÃ¨res Ã  la fonctions
		// qui gÃ¨re les interruption de l'uart
		rxBuffer[rxBufferFin] = getc(stdin);
        rxBufferFin = (rxBufferFin + 1) % RX_BUFFER_SIZE;
	}
	return NULL;
}

void init_hardware()
{
	pthread_t thread_RX;
	int ret;

	ret = pthread_create (&thread_RX, NULL, fake_RX, NULL);
	if (ret != 0)
		fprintf(stderr, "erreur %d\n", ret);
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
{}

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

}
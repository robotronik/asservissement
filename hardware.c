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
#include "SDL/affichage.h"

int PWM_D;
int PWM_G;
int x=50;
int y=50;
int a=0;

void set_PWM_moteur_D(int PWM)
{
	PWM_D=PWM;
}

void set_PWM_moteur_G(int PWM)
{
	PWM_G=PWM;
	actualise_pos();
}

void actualise_pos()
{
	int delta=(PWM_G+PWM_D)/2;
	int alpha=(PWM_D-PWM_G)/10;
	a+=alpha;
	x-=sin(alpha)*delta/alpha;
	y-=cos(alpha)*delta/alpha;
	set_position(x, y, a);
}

int get_nbr_tick_D()
{
	return PWM_D*100;
}

int get_nbr_tick_G()
{
	return PWM_G*100;
}

int doit_attendre()
{
	return 0;
}
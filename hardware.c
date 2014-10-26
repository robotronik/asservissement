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
#include <stdio.h> //à virer

int PWM_D;
int PWM_G;
int x_actuel=0;
int y_actuel=0;
int theta_actuel=0;

void set_PWM_moteur_D(int PWM)
{
	PWM_D=PWM;
}

void set_PWM_moteur_G(int PWM)
{
	PWM_G=PWM;
	actualise_pos();
}

void actualise_pos() //à dégager
{
	int delta=(PWM_G+PWM_D)/2;
	int alpha=(PWM_D-PWM_G)/10;

	//calcul dans le repère local
	int x_local,y_local;
	if(alpha!=0)
	{
		x_local=-(1-cos(alpha))*delta/alpha; //delta/alpha=R
		y_local=sin(alpha)*delta/alpha;
	}
	else
	{
		x_local=0;
		y_local=delta;
	}

	//rotation d'angle theta pour trouver la position en absolu
	x_actuel+=cos(theta_actuel)*x_local-sin(theta_actuel)*y_local;
	y_actuel+=sin(theta_actuel)*x_local+cos(theta_actuel)*y_local;
	//on actualise le reste
	theta_actuel+=alpha;
	set_position(x_actuel, y_actuel, theta_actuel);
	//printf("%d %d\n",x_actuel,y_actuel,theta_actuel);
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
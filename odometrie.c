#include "odometrie.h"
#include "hardware.h"
#include "reglages.h"
#include <math.h> // utiliser une autre methode pour calculer sin et cos ? (tableau ?)

//ici les calculs de position actuelle

//oh les vilaines variables globales !
int x_actuel; //absolu
int y_actuel; //absolu
int theta_actuel; //absolu
int delta_actuel; //relatif
int alpha_actuel; //relatif


void init_odometrie()
{
	x_actuel=0;
	y_actuel=0;
	delta_actuel=0;
	alpha_actuel=0;
	theta_actuel=0;
}

void set_delta_actuel(int delta)
{
	delta_actuel=delta;
}
void set_alpha_actuel(int alpha)
{
	alpha_actuel=alpha;
}

int get_delta_actuel()
{
	return delta_actuel;
}
int get_alpha_actuel()
{
	return alpha_actuel;
}

int get_theta_actuel()
{
	return theta_actuel;
}

int get_x_actuel()
{
	return x_actuel;
}
int get_y_actuel()
{
	return y_actuel;
}

void actualise_position()
{
	int nbr_tick_D=get_nbr_tick_D();
	int nbr_tick_G=get_nbr_tick_G();
	int delta=(nbr_tick_D+nbr_tick_G)/2/TICK_PAR_MM;
	int alpha=(nbr_tick_D-nbr_tick_G)/TICK_PAR_TOUR*DEUX_PI;
	//calcul également possible :
	//int alpha=(nbr_tick_D-nbr_tick_G)/TICK_PAR_MM/DEMI_ENTRAXE;

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
	delta_actuel+=delta;
	alpha_actuel+=alpha;
	theta_actuel+=alpha;
}
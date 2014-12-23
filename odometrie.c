#include "odometrie.h"
#include "hardware.h"
#include "reglages.h"
#include <math.h>
#include "math_precalc.h"
#include <stdio.h> //à virer
#include "trajectoire.h" //à virer
#define AFFICHAGE_DEBUG 0

//ici les calculs de position actuelle

//oh les vilaines variables globales !
static int x_actuel; //absolu
static int y_actuel; //absolu
static int theta_actuel; //absolu
static int delta_actuel; //relatif
static int alpha_actuel; //relatif


void init_odometrie()
{
	x_actuel=140;
	y_actuel=140;
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
	int nbr_tick_D=get_nbr_tick_D(); //long pour utilisation ac PIC ?
	int nbr_tick_G=get_nbr_tick_G(); //long pour utilisation ac PIC ?

	int delta=(nbr_tick_D+nbr_tick_G)/2; 	//delta en tick
		delta/=TICK_PAR_MM;					//convertion en mm

	int alpha=(nbr_tick_D-nbr_tick_G)/2; 	//alpha en ticks
		alpha*=1000;						//convertion en milliticks
		alpha*=DEUX_PI/TICK_PAR_TOUR; 		//convertion en milliradians

	//calcul également possible :
	//int alpha=(nbr_tick_D-nbr_tick_G)/TICK_PAR_MM/DEMI_ENTRAXE;

	//calcul dans le repère local
	double x_local,y_local;
	if(alpha!=0)
	{
		//delta/alpha=Rayon l'arc de cercle effectué (est-ce vraiment un arc de cercle ??)
		x_local=/*(int)*/ (1.0-cos_precalc(alpha))*delta/(alpha/1000.0);
		y_local=/*(int)*/ sin_precalc(alpha)*delta/(alpha/1000.0);
	}
	else
	{
		x_local=0;
		y_local=(double)delta;
	}

	//rotation d'angle theta pour trouver la position en absolu
	/*x_actuel+=(int) (cos((double)theta_actuel/1000.0)*x_local);
	x_actuel-=(int) (sin((double)theta_actuel/1000.0)*y_local);
	y_actuel+=(int) (sin((double)theta_actuel/1000.0)*x_local);
	y_actuel+=(int) (cos((double)theta_actuel/1000.0)*y_local);*/
	x_actuel+=(int) (cos_precalc(theta_actuel)*x_local);
	x_actuel-=(int) (sin_precalc(theta_actuel)*y_local);
	y_actuel+=(int) (sin_precalc(theta_actuel)*x_local);
	y_actuel+=(int) (cos_precalc(theta_actuel)*y_local);
	//on actualise le reste
	delta_actuel+=delta;
	alpha_actuel+=alpha;
	theta_actuel+=alpha;
	
	//TODO : faire attention à ce que theta reste borné
	//TODO : verifier que ça le code ci-après est correct (on met theta entre -pi et pi)
	/*
	theta_actuel+=(int)(PI*1000.0);
	theta_actuel%=((int)(DEUX_PI*1000.0));
	theta_actuel-=(int)(PI*1000.0);*/

	theta_actuel%=((int)(DEUX_PI*1000.0));
	if (theta_actuel>(int)(PI*1000.0))
	{
		theta_actuel-=(int)(DEUX_PI*1000.0);
	}
	else if (theta_actuel<-(int)(PI*1000.0))
	{
		theta_actuel+=(int)(DEUX_PI*1000.0);
	}

	//debug à virer
	if (AFFICHAGE_DEBUG == 1)
        printf("D_act:%d a_act:%d th_act:%d D_voul:%d a_voul:%d\n\n",delta_actuel,alpha_actuel,theta_actuel, get_delta_voulu(), get_alpha_voulu()); //à virer
}

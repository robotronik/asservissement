#include "odometrie.h"
#include "hardware.h"
#include "reglages.h"
#include "math_precalc.h"
#include <stdio.h> //à virer
#include "trajectoire.h" //à virer
#define AFFICHAGE_DEBUG 0

void actualise_xy(int d_delta, int d_alpha, int theta, float * x, float * y);
int borne_angle(long int angle);

//ici les calculs de position actuelle

//oh les vilaines variables globales !
static float x_actuel; //absolu
static float y_actuel; //absolu
static int theta_actuel; //absolu
static long int delta_actuel; //relatif
static long int alpha_actuel; //relatif


void init_odometrie()
{
	x_actuel=140;
	y_actuel=140;
	delta_actuel=0;
	alpha_actuel=0;
	theta_actuel=0;
}

void actualise_position()
{
	long int nbr_tick_D=get_nbr_tick_D();
	long int nbr_tick_G=get_nbr_tick_G();

	//calcul de alpha et delta apparents
	long int delta_lu=delta_mm(nbr_tick_D,nbr_tick_G);
	long int alpha_lu=alpha_millirad(nbr_tick_D,nbr_tick_G);

	//calcul des variations
	int d_delta=(int) (delta_lu-delta_actuel);
	int d_alpha=(int) (alpha_lu-alpha_actuel);

	//on actualise x et y actuels
	actualise_xy(d_delta,d_alpha,theta_actuel,&x_actuel,&y_actuel);
	
	//on actualise le reste
	delta_actuel=delta_lu;
	alpha_actuel=alpha_lu;
	theta_actuel=borne_angle(alpha_lu);

	//debug à virer
	if (AFFICHAGE_DEBUG == 1)
		printf("D_act:%ld a_act:%ld th_act:%d D_voul:%d a_voul:%d\n\n",delta_actuel,alpha_actuel,theta_actuel, get_delta_voulu(), get_alpha_voulu()); //à virer
}

long int delta_mm(long int nbr_tick_D, long int nbr_tick_G)
{
	long int delta;

	delta=(nbr_tick_D+nbr_tick_G)/2; 	//delta en tick
	delta/=TICK_PAR_MM;					//convertion en mm

	return delta;
}

long int alpha_millirad(long int nbr_tick_D, long int nbr_tick_G)
{
	long int alpha;

	alpha=(nbr_tick_D-nbr_tick_G)/2; 	//alpha en ticks
	alpha*=1000;						//convertion en milliticks
	alpha*=DEUX_PI/TICK_PAR_TOUR; 		//convertion en milliradians

	return alpha;
}

void actualise_xy(int d_delta, int d_alpha, int theta, float * x, float * y)
{
	//calcul des variations dans le repère local
	float d_x_local,d_y_local;
	if(d_alpha!=0)
	{
		//d_delta/(d_alpha/1000)=Rayon de "l'arc de cercle" effectué
		d_x_local=(1.0-cos_precalc(d_alpha))*(d_delta/(d_alpha/1000.0));
		d_y_local=sin_precalc(d_alpha)*(d_delta/(d_alpha/1000.0));
	}
	else
	{
		d_x_local=0;
		d_y_local=(float) d_delta;
	}

	//rotation selon l'orientation du robot pour trouver la position en absolu
	*x+=(cos_precalc(theta)*d_x_local) - (sin_precalc(theta)*d_y_local);
	*y+=(sin_precalc(theta)*d_x_local) + (cos_precalc(theta)*d_y_local);
}

int borne_angle(long int angle)
{
	//on borne "angle" entre pi et -pi

	angle%=((int)(DEUX_PI*1000.0));
	if (angle>(int)(PI*1000.0))
	{
		angle-=(int)(DEUX_PI*1000.0);
	}
	else if (angle<-(int)(PI*1000.0))
	{
		angle+=(int)(DEUX_PI*1000.0);
	}

	return (int) angle;
}

long int get_delta_actuel()
{
	return delta_actuel;
}

long int get_alpha_actuel()
{
	return alpha_actuel;
}

int get_theta_actuel()
{
	return theta_actuel;
}

int get_x_actuel()
{
	return (int) x_actuel;
}

int get_y_actuel()
{
	return (int) y_actuel;
}
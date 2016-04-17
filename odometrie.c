#include "odometrie.h"
#include "hardware.h"
#include "reglages.h"
#include "maths_utils.h"
#include "../hardware/debug.h" //à virer
#include "trajectoire.h" //à virer

void actualise_xy(int d_delta, int d_alpha, s_position * position_actuelle);//int theta, float * x, float * y);
int borne_angle(long int angle);

//ici les calculs de position actuelle

//doit disparaitre à terme
static s_position position_actuelle;

/*
void init_odometrie(s_position * position_actuelle)
{
	position_actuelle->x=X_INIT;
	position_actuelle->y=Y_INIT;
	position_actuelle->delta=0;
	position_actuelle->alpha=0;
	position_actuelle->theta=THETA_INIT;
}*/

void init_odometrie()
{
	position_actuelle.x=X_INIT;
	position_actuelle.y=Y_INIT;
	position_actuelle.delta=0;
	position_actuelle.alpha=0;
	position_actuelle.theta=THETA_INIT;
}

void actualise_position(s_position * position_actuelle)
{
	long int nbr_tick_D=get_nbr_tick_D();
	long int nbr_tick_G=get_nbr_tick_G();

	//correction
	nbr_tick_D*=COEFF_CODEUR_D;
	nbr_tick_G*=COEFF_CODEUR_G;

	//sauvegarde de l'état précédant
	long int delta_precedant=position_actuelle->delta;
	long int alpha_precedant=position_actuelle->alpha;

	//calcul de alpha et delta actuels
	position_actuelle->delta=delta_mm(nbr_tick_D,nbr_tick_G);
	position_actuelle->alpha=alpha_millirad(nbr_tick_D,nbr_tick_G);

	//calcul des variations
	int d_delta=(int) ((position_actuelle->delta)-delta_precedant);
	int d_alpha=(int) ((position_actuelle->alpha)-alpha_precedant);

	//on actualise x et y actuels
	actualise_xy(d_delta,d_alpha,position_actuelle);//theta_actuel,&x_actuel,&y_actuel);

	//on actualise l'orientation (theta)
	//theta_actuel=borne_angle(alpha_lu);
	//TODO : vérifier
	position_actuelle->theta=borne_angle((position_actuelle->theta)+d_alpha);
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

void actualise_xy(int d_delta, int d_alpha, s_position * position_actuelle)// int theta, float * x, float * y)
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

	//rotation selon l'orientation pour remonter aux variations réelles
	int theta=position_actuelle->theta;
	int d_x=(cos_precalc(theta)*d_x_local) - (sin_precalc(theta)*d_y_local);
	int d_y=(sin_precalc(theta)*d_x_local) + (cos_precalc(theta)*d_y_local);

	position_actuelle->x+=d_x;
	position_actuelle->y+=d_y;
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
	return position_actuelle.delta;
}

long int get_alpha_actuel()
{
	return position_actuelle.alpha;
}

int get_theta_actuel()
{
	return position_actuelle.theta;
}

int get_x_actuel()
{
	return (int) position_actuelle.x;
}

int get_y_actuel()
{
	return (int) position_actuelle.y;
}
float float_get_x_actuel()
{
	return position_actuelle.x;
}

float float_get_y_actuel()
{
	return position_actuelle.y;
}

s_position * get_position_actuelle()
{
	return &position_actuelle;
}

void set_theta_actuel(int new_theta)
{
	position_actuelle.theta=new_theta;
}

void set_x_actuel(int new_x)
{
	position_actuelle.x= (float) new_x;
}

void set_y_actuel(int new_y)
{
	position_actuelle.y= (float) new_y;
}

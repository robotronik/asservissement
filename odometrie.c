#include "odometrie.h"
#include "hardware.h"
#include "reglages.h"
#include "math_precalc.h"
#include "../hardware/debug.h" //à virer
#include "trajectoire.h" //à virer

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
	x_actuel=X_INIT;
	y_actuel=Y_INIT;
	delta_actuel=0;
	alpha_actuel=0;
	theta_actuel=THETA_INIT;
}

void actualise_position()
{
	long int nbr_tick_D=get_nbr_tick_D();
	long int nbr_tick_G=get_nbr_tick_G();

	//correction
	nbr_tick_D*=COEFF_CODEUR_D;
	nbr_tick_G*=COEFF_CODEUR_G;

	//sauvegarde de l'état précédant
	long int delta_precedant=delta_actuel;
	long int alpha_precedant=alpha_actuel;

	//calcul de alpha et delta actuels
	delta_actuel=delta_mm(nbr_tick_D,nbr_tick_G);
	alpha_actuel=alpha_millirad(nbr_tick_D,nbr_tick_G);

	//calcul des variations
	int d_delta=(int) (delta_actuel-delta_precedant);
	int d_alpha=(int) (alpha_actuel-alpha_precedant);

	//on actualise x et y actuels
	actualise_xy(d_delta,d_alpha,theta_actuel,&x_actuel,&y_actuel);

	//on actualise l'orientation (theta)
	//theta_actuel=borne_angle(alpha_lu);
	theta_actuel=borne_angle(theta_actuel+d_alpha); //TODO : vérifier

	debug(_VERBOSE_, "D_act:%ld a_act:%ld th_act:%d D_voul:%d a_voul:%d\n\n",delta_actuel,alpha_actuel,theta_actuel, get_delta_voulu(), get_alpha_voulu()); //à virer

	//on envoie notre position au PC (débug)
	//NB: vu que le traitement un peu long, je ne l'active que si le debug
	//est actif (de toute façon il ne se passe rien si DEBUG n'est pas
	//actif
#if DEBUG
	static int prev_x	  = 0;
	static int prev_y	  = 0;
	static int prev_theta = 0;

	int current_x	  = get_x_actuel();
	int current_y	  = get_y_actuel();
	int current_theta = get_theta_actuel();

	if (prev_x != current_x ||
		prev_y != current_y ||
		prev_theta != current_theta)
	{
		debug(_DEBUG_, "position actuelle : x=%f y=%f, theta=%d\n",
		(double)x_actuel, (double)y_actuel,theta_actuel);
	}
	prev_x	   = current_x;
	prev_y	   = current_y;
	prev_theta = current_theta;
#endif
}

long int delta_mm(long int nbr_tick_D, long int nbr_tick_G)
{
	long int delta;

	delta=(nbr_tick_D+nbr_tick_G)/2 	//delta en tick
		/ TICK_PAR_MM;					//convertion en mm

	return delta;
}

long int alpha_millirad(long int nbr_tick_D, long int nbr_tick_G)
{
	long int alpha;

	alpha=(nbr_tick_D-nbr_tick_G)	//alpha en ticks
		 * PI_MILLI/TICK_PAR_TOUR;	//convertion en milliradians

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

	angle%=DEUX_PI_MILLI;
	if (angle>PI_MILLI)
	{
		angle-=DEUX_PI_MILLI;
	}
	else if (angle<-PI_MILLI)
	{
		angle+=DEUX_PI_MILLI;
	}

	return angle;
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
	return x_actuel;
}

int get_y_actuel()
{
	return y_actuel;
}
float float_get_x_actuel()
{
	return x_actuel;
}

float float_get_y_actuel()
{
	return y_actuel;
}

void set_theta_actuel(int new_theta)
{
	theta_actuel=new_theta;
}

void set_x_actuel(int new_x)
{
	x_actuel= new_x;
}

void set_y_actuel(int new_y)
{
	y_actuel= new_y;
}

#include <math.h> //utiliser un tableau pour acos ??
#include <stdio.h> //TODO : à virer

#include "../hardware/debug.h"
#include "../common_code/communication/s2a_reception.h"

#include "trajectoire.h"
#include "odometrie.h"
#include "asser.h"
#include "hardware.h"
#include "reglages.h"
#include "maths_utils.h"

void update_consigne(s_trajectoire * trajectoire);

void process_trajectoire_alpha_delta(s_trajectoire * trajectoire);
void process_trajectoire_xy_relatif(s_trajectoire * trajectoire);
void process_trajectoire_xy_absolu(s_trajectoire * trajectoire);
void process_trajectoire_theta(s_trajectoire * trajectoire);
void process_trajectoire_chemin(s_trajectoire * trajectoire);
void calcul_alpha_delta_restant(int x_voulu, int y_voulu, int * new_alpha, int * new_delta);

void set_consigne_alpha_delta(int new_alpha, int new_delta);

//et encore de vilaines variables globales !
static s_trajectoire trajectoire_loc;
 s_position consigne;

void start_asser()
{
	unsigned char c;
	while(!arret()) //sur les robots on n'arrete jamais
	{
		//TODO : remove
		s_position * position_actuelle=get_position_actuelle();
		//on recalcule la position actuelle du robot (via les roues codeuses)
		actualise_position(position_actuelle);

		//on met à jour la consigne pour l'asser
		s_trajectoire * trajectoire=get_trajectoire();
		update_consigne(trajectoire);

		//asservissement
		s_position pos_consigne;
		pos_consigne.delta = consigne.delta;
		pos_consigne.alpha = consigne.alpha;
		asser(&pos_consigne, position_actuelle);


		//on regarde si on a pas reçu quelquechose
		if (UART_getc(&c)) {
			s2a_lecture_message((char) c);
		}
	}
}

void update_consigne(s_trajectoire * trajectoire)
{
	switch (trajectoire->type)
	{
		case alpha_delta :
			process_trajectoire_alpha_delta(trajectoire);
			break;
		case theta :
			process_trajectoire_theta(trajectoire);
			break;
		case xy_relatif :
			process_trajectoire_xy_relatif(trajectoire);
			break;
		case xy_absolu :
			process_trajectoire_xy_absolu(trajectoire);
			break;
		case chemin :
			process_trajectoire_chemin(trajectoire);
			break;
		case stop :
			process_trajectoire_alpha_delta(trajectoire);
			break;
		case emergency_stop :
			motors_stop(); //dans hardware_*.c
			break;
		case null :
			//il n'y a plus rien a faire
			break;
	}
}

void process_trajectoire_alpha_delta(s_trajectoire * trajectoire)
{
	int new_alpha=trajectoire->alpha;
	int new_delta=trajectoire->delta;
	set_consigne_alpha_delta(new_alpha,new_delta);
	trajectoire->type=null;
}

void process_trajectoire_theta(s_trajectoire * trajectoire)
{
	int theta_voulu=trajectoire->theta;
	int new_alpha=theta_voulu-get_theta_actuel();
	//on borne *new_alpha entre pi et -pi
	new_alpha=new_alpha%((int)(DEUX_PI*1000.0));
	if (new_alpha>(int)((DEUX_PI*1000.0)/2.0))
	{
		new_alpha-=(int)(DEUX_PI*1000.0);
	}
	else if (new_alpha<-(int)((DEUX_PI*1000.0)/2.0))
	{
		new_alpha+=(int)(DEUX_PI*1000.0);
	}
	set_consigne_alpha_delta(new_alpha,0);
	trajectoire->type=null;
}

void process_trajectoire_xy_relatif(s_trajectoire * trajectoire)
{
	int x_voulu=trajectoire->x;
	int y_voulu=trajectoire->y;
	trajectoire->x=x_voulu+get_x_actuel();
	trajectoire->y=y_voulu+get_y_actuel();
	trajectoire->type=xy_absolu;
	process_trajectoire_xy_absolu(trajectoire);
}

void process_trajectoire_xy_absolu(s_trajectoire * trajectoire)
{
	int x_voulu=trajectoire->x;
	int y_voulu=trajectoire->y;
	int new_alpha;
	int new_delta;
	x_voulu-=get_x_actuel();
	y_voulu-=get_y_actuel();
	calcul_alpha_delta_restant(x_voulu, y_voulu, &new_alpha, &new_delta);

	if(trajectoire->mode==courbe)
	{
		//évite que le robot ne tourne pour rien quand il a atteint xy avec la précision voulue
		if (new_delta<-PRECISION_DELTA || PRECISION_DELTA<new_delta)
		{
			set_consigne_alpha_delta(new_alpha,new_delta);
		}
		else
		{
			trajectoire->type=stop;
		}
	}
	else if(trajectoire->mode==tendu)
	{
		//on tourne pour se mettre dans la bonne direction
		if (new_alpha<-PRECISION_ALPHA || PRECISION_ALPHA<new_alpha)
		{
			set_consigne_alpha_delta(new_alpha,0);
		}
		//puis on avance tout droit (/!\ perte de précision)
		else
		{
			trajectoire->type=alpha_delta;
			trajectoire->delta=new_delta;
			trajectoire->alpha=0;
		}
	}
}

void process_trajectoire_chemin(s_trajectoire * trajectoire)
{
	s_liste liste_positions=trajectoire->chemin;
	if(trajectoire->chemin.taille==0)
	{
		trajectoire->type=stop;
		return;
	}

	//calcul de la consigne
	int new_alpha;
	int new_delta;
	int x_voulu=liste_positions.point[liste_positions.taille-1].x-get_x_actuel();
	int y_voulu=liste_positions.point[liste_positions.taille-1].y-get_y_actuel();
	calcul_alpha_delta_restant(x_voulu, y_voulu, &new_alpha, &new_delta);
	int sgn_delta=(new_delta > 0) - (new_delta < 0);

	//gestion du changement de point "cible"
	if (new_delta<-PRECISION_DELTA || PRECISION_DELTA<new_delta)
	{
		//si il n'y a plus qu'un point la vitesse ne doit plus être constante, il faut ralentir
		if(trajectoire->chemin.taille==1)
			set_consigne_alpha_delta(new_alpha,new_delta);
		else
			set_consigne_alpha_delta(new_alpha,sgn_delta*CONSTANTE_DELTA);
	}
	else
	{
		trajectoire->chemin.taille-=1;
	}
}

void calcul_alpha_delta_restant(int x_voulu, int y_voulu, int * new_alpha, int * new_delta)
{
	//x et y sont relatifs mais l'orientation des axes reste absolue

	//un peu moche...
	long int x_voulu_l=x_voulu;
	long int y_voulu_l=y_voulu;

	//voir si pas meilleur moyen pour le calcul de sqrt
	double distance = sqrt((double)(x_voulu_l*x_voulu_l+y_voulu_l*y_voulu_l));
	*new_delta=(int) distance;
	int sgn_x=(x_voulu >= 0) - (x_voulu < 0);
	 //voir si pas meilleur moyen pour le calcul de acos (tableau ?)
	int theta_voulu=(int)(1000.0*acos((double)(y_voulu)/distance)*(-1.0*sgn_x));
	*new_alpha=theta_voulu-get_theta_actuel();

	//on borne *new_alpha (à tester) entre pi et -pi
	*new_alpha=*new_alpha%((int)(DEUX_PI*1000.0));
	if (*new_alpha>(int)((DEUX_PI*1000.0)/2.0))
	{
		*new_alpha-=(int)(DEUX_PI*1000.0);
	}
	else if (*new_alpha<-(int)((DEUX_PI*1000.0)/2.0))
	{
		*new_alpha+=(int)(DEUX_PI*1000.0);
	}

	//TODO : gestion point non atteignable
	//(si l'on demande un point trop prés du robot et à la perpendiculaire de la direction du robot il se met à tourner autour du point)

	//gestion de la marche arrière
	if (*new_alpha>1571) //1571~=(pi/2)*1000
	{
		*new_alpha-=3142; //3142~=pi*1000
		*new_delta=-*new_delta;
	}
	else if (*new_alpha<-1571) //1571~=(pi/2)*1000
	{
		*new_alpha+=3142; //3142~=pi*1000
		*new_delta=-*new_delta;
	}
}

void build_trajectoire_alpha_delta(s_trajectoire * trajectoire, int alpha, int delta)
{
	trajectoire->type=alpha_delta;
	trajectoire->alpha=alpha;
	trajectoire->delta=delta;
}

void build_trajectoire_xy_relatif(s_trajectoire * trajectoire, int x, int y)
{
	trajectoire->type=xy_relatif;
	trajectoire->x=x;
	trajectoire->y=y;
}

void build_trajectoire_xy_absolu(s_trajectoire * trajectoire, int x, int y)
{
	trajectoire->type=xy_absolu;
	trajectoire->x=x;
	trajectoire->y=y;
}

void build_trajectoire_theta(s_trajectoire * trajectoire, int new_theta)
{
	trajectoire->type=theta;
	trajectoire->theta=new_theta;
}

void build_trajectoire_chemin(s_trajectoire * trajectoire, s_liste liste_positions)
{
	trajectoire->type=chemin;
	trajectoire->chemin=liste_positions;
}

void set_consigne_alpha_delta(int new_alpha, int new_delta)
{
	consigne.alpha=new_alpha+get_alpha_actuel();
	consigne.delta=new_delta+get_delta_actuel();
}

void build_trajectoire_stop(s_trajectoire * trajectoire)
{
	trajectoire->type=stop;
	build_trajectoire_alpha_delta(trajectoire,0,0);
}

void build_trajectoire_emergency_stop(s_trajectoire * trajectoire)
{
	trajectoire->type=emergency_stop;
}

// TODO : renomer
void set_trajectoire_mode2(s_trajectoire * trajectoire, e_mode_deplacement mode)
{
	trajectoire->mode=mode;
}

//TODO : renomer
void init_trajectoire2(s_trajectoire * trajectoire)
{
	trajectoire->type=null;
	set_consigne_alpha_delta(0,0);
	set_trajectoire_mode2(trajectoire, MODE_TRAJECTOIRE);
}

//TODO : remove
void init_trajectoire()
{
	trajectoire_loc.type=null;
	set_consigne_alpha_delta(0,0);
	set_trajectoire_mode2(&trajectoire_loc, MODE_TRAJECTOIRE);
}

s_trajectoire * get_trajectoire()
{
	return &trajectoire_loc;
}

//TODO : remove
void set_trajectoire_alpha_delta(int alpha, int delta)
{
	trajectoire_loc.type=alpha_delta;
	trajectoire_loc.alpha=alpha;
	trajectoire_loc.delta=delta;
}

void set_trajectoire_xy_relatif(int x, int y)
{
	trajectoire_loc.type=xy_relatif;
	trajectoire_loc.x=x;
	trajectoire_loc.y=y;
}

void set_trajectoire_xy_absolu(int x, int y)
{
	trajectoire_loc.type=xy_absolu;
	trajectoire_loc.x=x;
	trajectoire_loc.y=y;
}

void set_trajectoire_theta(int new_theta)
{
	trajectoire_loc.type=theta;
	trajectoire_loc.theta=new_theta;
}

void set_trajectoire_chemin(s_liste liste_positions)
{
	trajectoire_loc.type=chemin;
	trajectoire_loc.chemin=liste_positions;
}

void set_trajectoire_stop()
{
	trajectoire_loc.type=stop;
	set_trajectoire_alpha_delta(0,0);
}

void set_trajectoire_emergency_stop()
{
	trajectoire_loc.type=emergency_stop;
}

void set_trajectoire_mode(e_mode_deplacement mode)
{
	trajectoire_loc.mode=mode;
}
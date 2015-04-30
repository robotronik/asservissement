#include <math.h> //utiliser un tableau pour acos ??
#include <stdio.h> //TODO : à virer

#include "../common_code/debug.h"

#if USE_SDL
#   include "../common_code/simulation/affichage.h"
#endif

#include "trajectoire.h"
#include "odometrie.h"
#include "asser.h"
#include "communication.h"
#include "match.h"
#include "hardware.h"
#if PIC_BUILD
#	if   GROS
#		include "reglages_gros.h"
#	elif PETIT
#		include "reglages_petit.h"
#	endif
#else
#	include "reglages_PC.h"
#endif

void update_consigne();

void make_trajectoire_alpha_delta(int new_alpha, int new_delta);
void make_trajectoire_xy_relatif(int x_voulu, int y_voulu);
void make_trajectoire_xy_absolu(int x_voulu, int y_voulu);
void make_trajectoire_theta(int theta_voulu);
void make_trajectoire_chemin(s_liste liste_positions);
void calcul_alpha_delta_restant(int x_voulu, int y_voulu, int * new_alpha, int * new_delta);

void set_consigne_alpha_delta(int new_alpha, int new_delta);

//et encore de vilaines variables globales !
static s_trajectoire trajectoire;
static s_consigne consigne;

void start()
{
	while(
#if USE_SDL
		sdl_manage_events()==0 &&
#endif
		match_get_etat() != MATCH_FIN)
	{
		//asservissement
		asser(consigne);

		//on recalcule la position actuelle du robot (via les roues codeuses)
		actualise_position();

		//on met à jour la consigne pour l'asser
		update_consigne();

		//test si synchro ok : attention bloquant à retirer pour la coupe
#if PIC_BUILD
		if(!attente_synchro())
		{
			motors_stop();
			while(1) {
				allumer_del();
				pause_ms(500);
				eteindre_del();
				pause_ms(500);
			}
		}
#endif
	}
}

void update_consigne()
{
	switch (trajectoire.type)
	{
		case alpha_delta :
			make_trajectoire_alpha_delta(trajectoire.alpha,trajectoire.delta);
			break;
		case theta :
			make_trajectoire_theta(trajectoire.theta);
			break;
		case xy_relatif :
			make_trajectoire_xy_relatif(trajectoire.x_relatif,trajectoire.y_relatif);
			break;
		case xy_absolu :
			make_trajectoire_xy_absolu(trajectoire.x_absolu,trajectoire.y_absolu);
			break;
		case chemin :
			make_trajectoire_chemin(trajectoire.chemin);
			break;
		case stop :
			make_trajectoire_alpha_delta(0,0);
			break;
		case emergency_stop :
			motors_stop(); //dans hardware_*.c
			break;
		case null :
			break;
	}
}

void make_trajectoire_alpha_delta(int new_alpha, int new_delta)
{
	set_consigne_alpha_delta(new_alpha,new_delta);
	trajectoire.type=null;
}

void make_trajectoire_theta(int theta_voulu)
{
	int new_alpha=theta_voulu-get_theta_actuel();
	set_consigne_alpha_delta(new_alpha,0);
	trajectoire.type=null;
}

void make_trajectoire_xy_relatif(int x_voulu, int y_voulu)
{
	trajectoire.x_absolu=x_voulu+get_x_actuel();
	trajectoire.y_absolu=y_voulu+get_y_actuel();
	trajectoire.type=xy_absolu;
	make_trajectoire_xy_absolu(trajectoire.x_absolu,trajectoire.y_absolu);
}

void make_trajectoire_xy_absolu(int x_voulu, int y_voulu)
{
	int new_alpha;
	int new_delta;
	x_voulu-=get_x_actuel();
	y_voulu-=get_y_actuel();
	calcul_alpha_delta_restant(x_voulu, y_voulu, &new_alpha, &new_delta);

	if(trajectoire.mode==courbe)
	{
		//évite que le robot ne tourne pour rien quand il a atteint xy avec la précision voulue
		if (new_delta<-PRECISION_DELTA || PRECISION_DELTA<new_delta)
		{
			set_consigne_alpha_delta(new_alpha,new_delta);
		}
		else
		{
			trajectoire.type=stop;
		}
	}
	else if(trajectoire.mode==tendu)
	{
		//on tourne pour se mettre dans la bonne direction
		if (new_alpha<-PRECISION_ALPHA || PRECISION_ALPHA<new_alpha)
		{
			set_consigne_alpha_delta(new_alpha,0);
		}
		//puis on avance tout droit (/!\ perte de précision)
		else
		{
			trajectoire.type=alpha_delta;
			trajectoire.delta=new_delta;
			trajectoire.alpha=0;
		}
	}
}

void make_trajectoire_chemin(s_liste liste_positions)
{
	if(trajectoire.chemin.taille==0)
	{
		trajectoire.type=stop;
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
		if(trajectoire.chemin.taille==1)
			set_consigne_alpha_delta(new_alpha,new_delta);
		else
			set_consigne_alpha_delta(new_alpha,sgn_delta*CONSTANTE_DELTA);
	}
	else
	{
		trajectoire.chemin.taille-=1;
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
	int sgn_x=(x_voulu > 0) - (x_voulu < 0);
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

void set_trajectoire_alpha_delta(int alpha, int delta)
{
	trajectoire.type=alpha_delta;
	trajectoire.alpha=alpha;
	trajectoire.delta=delta;
}

void set_trajectoire_xy_relatif(int x, int y)
{
	trajectoire.type=xy_relatif;
	trajectoire.x_relatif=x;
	trajectoire.y_relatif=y;
}

void set_trajectoire_xy_absolu(int x, int y)
{
	trajectoire.type=xy_absolu;
	trajectoire.x_absolu=x;
	trajectoire.y_absolu=y;
}

void set_trajectoire_theta(int new_theta)
{
	trajectoire.type=theta;
	trajectoire.theta=new_theta;
}

void set_trajectoire_chemin(s_liste liste_positions)
{
	trajectoire.type=chemin;
	trajectoire.chemin=liste_positions;
}

void set_consigne_alpha_delta(int new_alpha, int new_delta)
{
	consigne.alpha=new_alpha+get_alpha_actuel();
	consigne.delta=new_delta+get_delta_actuel();
}

void set_trajectoire_stop()
{
	trajectoire.type=stop;
}

void set_trajectoire_emergency_stop()
{
	trajectoire.type=emergency_stop;
}

void set_trajectoire_mode(e_mode_deplacement mode)
{
	trajectoire.mode=mode;
}

void init_trajectoire()
{
	trajectoire.type=null;
	set_consigne_alpha_delta(0,0);
	set_trajectoire_mode(MODE_TRAJECTOIRE);
}

//TODO : à virer (utile uniquement pour du debug)
int get_delta_voulu()
{
	return consigne.delta;
}

int get_alpha_voulu()
{
	return consigne.alpha;
}

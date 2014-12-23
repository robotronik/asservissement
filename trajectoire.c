#include "trajectoire.h"
#include "odometrie.h"
#include "reglages.h"
#include "asser.h"
#include "debug/affichage.h" //à virer
#include <math.h> //utiliser un tableau pour acos ??
#include <stdio.h> //TODO : à virer

//ici le calcul de la trajectoire à effectuer

//et encore de vilaines variables globales !
static int delta_voulu;
static int alpha_voulu;

/*la gestion des changements successifs d'alpha et beta pour atteindre
**la position en xy voulu est relativement moche -> TODO : reorganiser*/
static int x_voulu_absolu;
static int y_voulu_absolu;
static int consigne_is_xy;

static s_trajectoire trajectoire;
static s_consigne consigne;

//TODO : réfléchir à l'idée suivante :
//quand on doit passer sur une liste de points donner une consigne beta constante
//et recalculer uniquement alpha sauf pour le dernier point où il faut ralentir
//de cette manière on ne ralentira pas en passant sur chaque points

void start()
{
	while(!sdl_manage_events())
	{
		update_consigne();
		asser();
	}
}

/*void update_consigne()
{
	switch (trajectoire.type)
	{
		case alpha_delta :
			consigne.delta=trajectoire.delta;
			consigne.alpha=trajectoire.alpha;
			break;
		case theta :
			consigne_new_theta();
			asser();
			break;
		case xy_absolu :
			consigne_new_xy_absolu();
			asser();
			break;
		case xy_relatif :
			consigne_new_xy_relatif();
			asser();
			break;
		case liste_xy :
			//releve le prochain point et donne un alpha à réliser
			//en gardant un beta constant si le point en question
			//n'est pas le dernier
			asser();
			break;
	}
}*/

void consigne_new_alpha_delta(int new_alpha, int new_delta)
{
	set_alpha_voulu(new_alpha);
	set_delta_voulu(new_delta);
	set_alpha_actuel(0);
	set_delta_actuel(0);
}

void consigne_new_xy_relatif(int x_voulu, int y_voulu)
{
	//x et y sont relatifs mais l'orientation ne change pas avec celle du robot
	set_x_voulu_absolu(x_voulu+get_x_actuel());
	set_y_voulu_absolu(y_voulu+get_y_actuel());

	//voir si pas meilleur moyen pour le calcul de sqrt
	int new_delta=(int)sqrt((double)(x_voulu*x_voulu+y_voulu*y_voulu));
	int sgn_x=(x_voulu > 0) - (x_voulu < 0);
	 //voir si pas meilleur moyen pour le calcul de acos (tableau ?)
	int theta_voulu=(int)(1000.0*acos((double)(y_voulu)/(double)(new_delta))*(-1.0*sgn_x));
	int new_alpha=theta_voulu-get_theta_actuel();

	//on borne new_alpha (à tester)
	new_alpha=new_alpha%((int)(DEUX_PI*1000.0));
	if (new_alpha>(int)((DEUX_PI*1000.0)/2.0))
	{
		new_alpha-=(int)(DEUX_PI*1000.0);
	}
	else if (new_alpha<-(int)((DEUX_PI*1000.0)/2.0))
	{
		new_alpha+=(int)(DEUX_PI*1000.0);
	}	

	//TODO : gestion point non atteignable
	//(si l'on demande un point trop prés du robot et à la perpendiculaire de la direction du robot il se met à tourner)
	
	//gestion de la marche arrière
	if (new_alpha>1571) //1571~=(pi/2)*1000
	{
		new_alpha-=3142; //3142~=pi*1000
		new_delta=-new_delta;
	}
	else if (new_alpha<-1571) //1571~=(pi/2)*1000
	{
		new_alpha+=3142; //3142~=pi*1000
		new_delta=-new_delta;
	}
	//évite que le robot ne tourne pour rien quand il a atteint xy avec la précision voulue
	if (new_delta<-PRECISION_DELTA || PRECISION_DELTA<new_delta)
	{
		consigne_new_alpha_delta(new_alpha,new_delta);
	}
	else
	{
		consigne_new_alpha_delta(0,0);
	}
}

void consigne_new_xy_absolu(int x_voulu, int y_voulu)
{
	consigne_new_xy_relatif(x_voulu-get_x_actuel(),y_voulu-get_y_actuel());
}

void consigne_new_theta(int theta_voulu)
{
	int new_alpha=theta_voulu-get_theta_actuel();
	consigne_new_alpha_delta(new_alpha,0);
}

void update_consigne()
{
	if (consigne_is_xy)
	{
		consigne_new_xy_relatif(x_voulu_absolu-get_x_actuel(),y_voulu_absolu-get_y_actuel());
	}
}

void init_trajectoire()
{
	init_alpha_delta_voulu();
	consigne_is_xy=0;
}

void init_alpha_delta_voulu()
{
	trajectoire.delta_voulu=0;
	trajectoire.alpha_voulu=0;
}

void consigne_type_is_xy(int a)
{
	consigne_is_xy=a;
}

void set_delta_voulu(int delta)
{
	delta_voulu=delta;
}

void set_alpha_voulu(int alpha)
{
	alpha_voulu=alpha;
}

void set_x_voulu_absolu(int x)
{
	x_voulu_absolu=x;
}

void set_y_voulu_absolu(int y)
{
	y_voulu_absolu=y;
}

int get_delta_voulu()
{
	return delta_voulu;
	//return consigne.delta;
}

int get_alpha_voulu()
{
	return alpha_voulu;
	//return consigne.alpha;
}

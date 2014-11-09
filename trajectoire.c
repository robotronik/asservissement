#include "trajectoire.h"
#include "odometrie.h"
#include <math.h> //utiliser un tableau pour acos ??

//ici le calcul de la trajectoire à effectuer

//et encore de vilaines variables globales !
int delta_voulu;
int alpha_voulu;

/*la gestion des changements successifs d'alpha et beta pour atteindre
**la position en xy voulu est relativement moche -> TODO : reorganiser*/
int x_voulu_absolu;
int y_voulu_absolu;
int consigne_is_xy;

void init_trajectoire()
{
	init_alpha_delta_voulu();
	consigne_is_xy=0;
}

void init_alpha_delta_voulu()
{
	delta_voulu=0;
	alpha_voulu=0;
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

void consigne_type_is_xy(int a)
{
	consigne_is_xy=a;
}

void set_y_voulu_absolu(int y)
{
	y_voulu_absolu=y;
}

int get_delta_voulu()
{
	return delta_voulu;
}

int get_alpha_voulu()
{
	return alpha_voulu;
}

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

	int new_delta=(int)sqrt((double)(x_voulu*x_voulu+y_voulu*y_voulu)); //voir si pas meilleur moyen
	int sgn_x=(x_voulu > 0) - (x_voulu < 0);
	int theta_voulu=(int)(1000.0*acos((double)(y_voulu)/(double)(new_delta))*(-1.0*sgn_x)); //voir si pas meilleur moyen (tableau ?)
	int new_alpha=theta_voulu-get_theta_actuel();
	consigne_new_alpha_delta(new_alpha,new_delta);
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

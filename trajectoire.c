#include "odometrie.h"
#include <math.h> //utiliser un tableau pour acos ??

//ici le calcul de la trajectoire à effectuer

//et encore de vilaines variables globales !
int delta_voulu;
int alpha_voulu;

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

	int new_delta=(int)sqrt((double)(x_voulu*x_voulu+y_voulu*y_voulu)); //voir si pas meilleur moyen
	//pas bon à revoir type de variable mauvais ou alors passer à un angle en degré -> passage en milliradian depuis
	int sgn_x=(x_voulu > 0) - (x_voulu < 0);
	//sgn_x=1;
	int theta_voulu=(int)(1000.0*acos((double)(y_voulu/new_delta))*(-1*sgn_x)); //voir si pas meilleur moyen (tableau ?)
	printf("%d\n", theta_voulu);
	int new_alpha=theta_voulu-get_theta_actuel();
	//acos retourne et prend un double à priori
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

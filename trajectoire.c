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
	int new_delta=sqrt(x_voulu*x_voulu+y_voulu*y_voulu); //voir si pas meilleur moyen
	int new_alpha=acos(x_voulu/new_delta); //voir si pas meilleur moyen (tableau ?)
	//acos retourne un double à priori
	consigne_new_alpha_delta(new_alpha,new_delta);
}

void consigne_new_xy_absolu(int x_voulu, int y_voulu)
{
	consigne_new_xy_relatif(x_voulu-get_x_actuel(),y_voulu-get_y_actuel());
}

void consigne_new_tetha(int tetha_voulu)
{
	int new_alpha=tetha_voulu-get_tetha_actuel();
	consigne_new_alpha_delta(new_alpha,0);
}

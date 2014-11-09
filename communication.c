#include "trajectoire.h"
#include "odometrie.h"
#include <stdio.h> //à virer
#include "debug/affichage.h" // à virer
#define AFFICHAGE_DEBUG 1

/*TODO : implémenter le protocole de comunication comme ça
avait été fait dans message.c pour le code présent dans "old"*/

void set_new_alpha_delta(int alpha, int delta)
{
	consigne_new_alpha_delta(alpha,delta);
}

void set_new_xy_relatif(int x, int y)
{
	consigne_new_xy_relatif(x,y);
}

void set_new_xy_absolu(int x, int y)
{
	consigne_new_xy_absolu(x,y);
}

void set_new_theta(int theta)
{
	consigne_new_theta(theta);
}

void send_position_atteinte()
{
    if (AFFICHAGE_DEBUG == 1)
        printf("atteint\n");
}

//void send_asser_pret() ??

void send_position_xbee() // ??
{
	set_position(get_x_actuel(), get_y_actuel(),get_theta_actuel()); //à virer
}

#include "trajectoire.h"
#include "odometrie.h"
#include <stdio.h> //à virer
#include "SDL/affichage.h" // à virer

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
 printf("atteint\n");
}

//void send_asser_pret() ??

void send_position_xbee()
{
	set_position(get_x_actuel(), 2000-get_y_actuel(),get_theta_actuel()/1000*180/3.14159+90); //à virer
}
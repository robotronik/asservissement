#include "asser.h"
#include "trajectoire.h"
#include "odometrie.h"
#include "hardware.h"
#include "debug/affichage.h"
#include <stdio.h>


int main()
{
    if (init_sdl_screen() < 0)
        return 1; 
    
    init_odometrie();
    init_trajectoire();
    init_hardware();
    init_asser();
    //new_alpha_delta(0,1000);
    s_liste chemin;
    chemin.taille=5;
    chemin.point[0].x=1000;
    chemin.point[0].y=1000;
    chemin.point[1].x=2000;
    chemin.point[1].y=800;
    chemin.point[2].x=1000;
    chemin.point[2].y=1000;
    chemin.point[3].x=250;
    chemin.point[3].y=1000;
    chemin.point[4].x=140; //position initiale du robot
    chemin.point[4].y=140; //position initiale du robot
    //set_trajectoire_chemin(chemin);
    //set_trajectoire_theta(10000*DEUX_PI);
    //set_trajectoire_alpha_delta(4595,0);
    set_trajectoire_xy_relatif(500,800);
    //set_trajectoire_xy_absolu(500,800);
    start();
	//asser();
	return quit_sdl_screen();
}
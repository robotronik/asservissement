#include "asser.h"
#include "trajectoire.h"
#include "odometrie.h"
#include "hardware.h"
#include "debug/affichage.h"

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
    chemin.taille=3;
    chemin.point[0].x=1000;
    chemin.point[0].y=1000;
    chemin.point[1].x=250;
    chemin.point[1].y=1000;
    chemin.point[2].x=0;
    chemin.point[2].y=0;
    set_trajectoire_chemin(chemin);
    //set_trajectoire_xy_relatif(0,0);
    //set_trajectoire_xy_absolu(0,0);
    start();
	//asser();
	return quit_sdl_screen();
}
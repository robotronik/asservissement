#include "hardware.h"
#include "asser.h"
#include "trajectoire.h"
#include "odometrie.h"
#include "tests_unitaires.h"

#if USE_SDL
#   include "../common_code/simulation/affichage.h"
#endif

int main()
{
	//init
	init_odometrie();
	init_trajectoire();
	init_hardware();
	init_asser();

	//lancement du robot
	//launch_tests();
	start();

	//évite un reset automatique du microcontrôleur
	#if PIC_BUILD
		while(1);
	#endif

	#if USE_SDL
		return quit_sdl_screen();
	#endif

	return 0;
}
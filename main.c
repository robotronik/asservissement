#include <stdio.h>
#include <stdbool.h>

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

#include "../common_code/communication/s2a_reception.h"

#include "asser.h"
#include "trajectoire.h"
#include "odometrie.h"
#include "tests_unitaires.h"
#if USE_SDL
#   include "../common_code/simulation/affichage.h" // Ã  virer
#endif
#include "match.h"

#include "../common_code/common.h"

int runTest();
int runMatch();

int main()
{
	/*init*/
	init_odometrie();
	init_trajectoire();
	init_hardware();
	init_asser();
	//launch_tests();
	runMatch();

	//évite un reset automatique du microcontrôleur
	#if PIC_BUILD
		while(1);
	#endif

	return 0;
}

int runMatch()
{
	#if PIC_BUILD
		start();
	#else
		#if USE_SDL
			if (init_sdl_screen() < 0)
				return 0;
			start();
			return quit_sdl_screen();
		#else
			start();
			return 0;
		#endif
	#endif

	return 0;
}
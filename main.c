#include "hardware.h"
#include "asser.h"
#include "trajectoire.h"
#include "odometrie.h"
#include "tests_unitaires.h"

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

	return 0;
}
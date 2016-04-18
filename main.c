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
	start_asser();

	//évite un reset automatique du microcontrôleur
	while(1);

	return 0;
}

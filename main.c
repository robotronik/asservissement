#include "hardware.h"
#include "asser.h"
#include "trajectoire.h"
#include "odometrie.h"
#include "tests_unitaires.h"

callback_t callbacks[KEYS_SIZE] = {
};
int main()
{
	//init
	init_odometrie();
	init_trajectoire();
	init_hardware();
	init_asser();

	init_reception(callbacks);


	//lancement du robot
	//launch_tests();
	start_asser();

	//évite un reset automatique du microcontrôleur
	while(1);

	return 0;
}

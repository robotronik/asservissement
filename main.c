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


	/* Blink the LEDs (PD12, PD13, PD14 and PD15) on the board. */
	while (1) {
		motors_on();
		allumer_del();
		for (int i = 0; i < 60000; i++) { /* Wait a bit. */
			__asm__("nop");
		}
		motors_stop();
		eteindre_del();
		for (int i = 0; i < 60000; i++) { /* Wait a bit. */
			__asm__("nop");
		}
	}

	return 0;

	//lancement du robot
	//launch_tests();
	start_asser();

	//évite un reset automatique du microcontrôleur
	while(1);

	return 0;
}

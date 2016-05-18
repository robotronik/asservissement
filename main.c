#include "hardware.h"
#include "asser.h"
#include "trajectoire.h"
#include "odometrie.h"
#include "tests_unitaires.h"
#include "reception_communication.h"

#include "hardware.h"
#include <debug.h>

#include <alarms_and_delay.h>

int main()
{
	//init
	init_odometrie();
	init_trajectoire();
	init_hardware();
	init_asser();
	init_reception_communication();


/*
	// Test moche de rampe de PWM
	while(1)
		for (int i = -20; i <= 40; i+=1) {
			set_PWM_moteur_D(i);
			set_PWM_moteur_G(i);
			char str[35];
			sprintf(str, "pwm : %d \r\n", i);
			UART_send_message(str, strlen(str));
			delay_ms(200);
		}
*/


	//test_sens_codeurs();
	//test_moteur_G(16000);
	//test_moteur_D(16000);
	//test_distance(500, 16000);
	//test_angle(DEUX_PI_MILLI,8000);
	//test_asser_alpha_delta(-PI_MILLI,000);
	//test_asser_alpha_delta(0, 500);
	//while(1);

	//lancement du robot
	//launch_tests();
        start_asser();

	//évite un reset automatique du microcontrôleur
	while(1);

	return 0;
}

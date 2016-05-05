#include <stdio.h>
#include <string.h>

#include "tests_unitaires.h"
#include "hardware.h"
#include "reglages.h"

#include "asser.h"
#include "odometrie.h"
#include "trajectoire.h"

#include <UART.h>

#include <alarms_and_delay.h>


void launch_tests()
{
	//tests de branchements

		//test du branchement des moteurs
		// test_moteur_D(400000);
		// test_moteur_G(400000);

		//test du branchement des codeurs
		// test_codeur_D();
		// test_codeur_G();

		//test du sens des codeurs (allume la led si marche avant)
		// test_sens_codeur_D();
		// test_sens_codeur_G();

	//tests pour réglage des parametres

		//tests en boucle ouverte
		// test_vitesse(MIN_VITESSE);
		// test_ecretage();
		// test_distance(10000,400000);
		// test_angle(3142,400000);

		//test d'asservissement
		// test_asser_alpha_delta(0,1000);
		// test_asser_alpha_delta(3142,0);
		// test_asser_theta(3142);
		// test_asser_xy_relatif_tendu(400,400);
		// test_asser_xy_absolu_tendu(140,400+140);
		// test_asser_xy_relatif_courbe(400,400);
		// test_asser_xy_absolu_courbe(140,400+140);

	//exemple de chemin de test
		// s_liste chemin;
		// chemin.taille=7;
		// chemin.point[0].x=140;
		// chemin.point[0].y=300;
		// chemin.point[1].x=540;
		// chemin.point[1].y=300;
		// chemin.point[2].x=540;
		// chemin.point[2].y=500;
		// chemin.point[3].x=2700;
		// chemin.point[3].y=1600;
		// chemin.point[4].x=2000;
		// chemin.point[4].y=800;
		// chemin.point[5].x=1000;
		// chemin.point[5].y=1000;
		// chemin.point[6].x=250;
		// chemin.point[6].y=1000;
		// chemin.point[3].x=140; //position initiale du robot
		// chemin.point[3].y=140; //position initiale du robot
		// test_asser_chemin(chemin);
}

void test_moteur_D(long int vitesse)
{
		int PWM_D=convert2PWM(COEFF_MOTEUR_D*vitesse);
		set_PWM_moteur_D(PWM_D);
}

void test_moteur_G(long int vitesse)
{
		int PWM_G=convert2PWM(COEFF_MOTEUR_G*vitesse);
		set_PWM_moteur_G(PWM_G);
}

void test_codeur_D()
{
	while(get_nbr_tick_D()==0){;}
	allumer_del();
}

void test_codeur_G()
{
	while(get_nbr_tick_G()==0){;}
	allumer_del();
}

void test_sens_codeur_D()
{
	while(1)
	{
		int nbr_tick_D=get_nbr_tick_D();
		if (nbr_tick_D>0)
		{
			allumer_del();
		}
		else
		{
			eteindre_del();
		}
	}
}

void test_sens_codeurs()
{
	while(1) {
		long int nbr_tick_G=get_nbr_tick_G();
		long int nbr_tick_D=get_nbr_tick_D();
		char str[35];
		sprintf(str, "g : %ld d : %ld \r\n", nbr_tick_G, nbr_tick_D);
		UART_send_message(str, strlen(str));
		if (nbr_tick_G>0)
			allumer_del();
		if (nbr_tick_G<0)
			eteindre_del();

		if (nbr_tick_D>0)
			allumer_autres_del();
		if (nbr_tick_D<0)
			eteindre_autres_del();

		delay_ms(100);
	}
}

void test_vitesse(long int vitesse)
{
		test_moteur_D(vitesse);
		test_moteur_G(vitesse);
}

void test_ecretage()
{
	long int commande;
	long int commande_preced=0;
	while(commande_preced!=MAX_VITESSE)
	{
		commande=MAX_VITESSE;
		ecretage(&commande,commande_preced);
		test_vitesse(commande);
		commande_preced=commande;
	}
	long int i;
	for(i=0;i<2000000;i++) {};
	while(commande_preced!=0)
	{
		commande=0;
		ecretage(&commande,commande_preced);
		test_vitesse(commande);
		commande_preced=commande;
	}
}

void test_distance(long int distance, long int vitesse)
{
	long int nbr_tick_D;
	long int nbr_tick_G;
	long int distance_actuelle=0;

	//si la distance est négative il faut changer de sens !
	if (distance<0)
	{
		vitesse=-vitesse;
	}

	test_vitesse(vitesse);
	while(abs(distance_actuelle)<abs(distance))
	{
		nbr_tick_D=get_nbr_tick_D();
		nbr_tick_G=get_nbr_tick_G();
		nbr_tick_D*=COEFF_CODEUR_D;
		nbr_tick_G*=COEFF_CODEUR_G;
		distance_actuelle=delta_mm(nbr_tick_D,nbr_tick_G);


		char str[100];
		sprintf(str, "g : %ld d : %ld, dist = %ld\r\n", nbr_tick_G, nbr_tick_D, distance_actuelle);
		UART_send_message(str, strlen(str));

	}
	test_vitesse(0);

}

void test_angle(long int angle, long int vitesse)
{
	long int nbr_tick_D;
	long int nbr_tick_G;
	long int angle_actuel=0;

	//si l'angle est négatif il faut changer de sens !
	if (angle<0)
	{
		vitesse=-vitesse;
	}

		test_moteur_D(vitesse);
		test_moteur_G(-vitesse);

	while(abs(angle_actuel)<abs(angle))
	{
		nbr_tick_D=get_nbr_tick_D();
		nbr_tick_G=get_nbr_tick_G();
		nbr_tick_D*=COEFF_CODEUR_D;
		nbr_tick_G*=COEFF_CODEUR_G;
		angle_actuel=alpha_millirad(nbr_tick_D,nbr_tick_G);
	}
	set_all_led();
	test_vitesse(0);
}

void test_asser_alpha_delta(int alpha, int delta)
{
	set_trajectoire_alpha_delta(alpha,delta);
	start_asser();
}

void test_asser_theta(int theta)
{
	set_trajectoire_theta(theta);
	start_asser();
}

void test_asser_xy_relatif_courbe(int x, int y)
{
	set_trajectoire_mode(courbe);
	set_trajectoire_xy_relatif(x,y);
	start_asser();
}

void test_asser_xy_absolu_courbe(int x, int y)
{
	set_trajectoire_mode(courbe);
	set_trajectoire_xy_absolu(x,y);
	start_asser();
}

void test_asser_xy_relatif_tendu(int x, int y)
{
	set_trajectoire_mode(tendu);
	set_trajectoire_xy_relatif(x,y);
	start_asser();
}

void test_asser_xy_absolu_tendu(int x, int y)
{
	set_trajectoire_mode(tendu);
	set_trajectoire_xy_absolu(x,y);
	start_asser();
}

void test_asser_chemin(s_liste chemin)
{
	set_trajectoire_chemin(chemin);
	start_asser();
}

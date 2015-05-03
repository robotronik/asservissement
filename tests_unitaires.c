#include "tests_unitaires.h"
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

#include "../common_code/communication/s2a_emission.h"

#include "asser.h"
#include "odometrie.h"
#include "trajectoire.h"


void test_vitesse(long int vitesse)
{
	int PWM=convert2PWM(vitesse);
	set_PWM_moteur_D(PWM);
	set_PWM_moteur_G(PWM);
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
		distance_actuelle=delta_mm(nbr_tick_D,nbr_tick_G);
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

	int PWM=convert2PWM(vitesse);
	set_PWM_moteur_D(PWM);
	set_PWM_moteur_G(-PWM);

	while(abs(angle_actuel)<abs(angle))
	{
		nbr_tick_D=get_nbr_tick_D();
		nbr_tick_G=get_nbr_tick_G();
		angle_actuel=alpha_millirad(nbr_tick_D,nbr_tick_G);
	}
	test_vitesse(0);
}

void test_asser_alpha_delta(int alpha, int delta)
{
	s2a_send_message(S2A_KEY_ALPHA, alpha);
	s2a_send_message(S2A_KEY_DELTA, delta);
	s2a_send_message(S2A_FCT_ALPHA_DELTA);
	start();
}

void test_asser_theta(int theta)
{
	s2a_send_message(S2A_KEY_THETA, theta);
	s2a_send_message(S2A_FCT_THETA);
	start();
}

void test_asser_xy_relatif_courbe(int x, int y)
{
	s2a_send_message(S2A_KEY_X, x);
	s2a_send_message(S2A_KEY_Y, y);
	s2a_send_message(S2A_FCT_XY_RELATIF);
	s2a_send_message(S2A_FCT_MODE_COURBE);
	start();
}

void test_asser_xy_absolu_courbe(int x, int y)
{
	s2a_send_message(S2A_KEY_X, x);
	s2a_send_message(S2A_KEY_Y, y);
	s2a_send_message(S2A_FCT_XY_ABSOLU);
	s2a_send_message(S2A_FCT_MODE_COURBE);
	start();
}

void test_asser_xy_relatif_tendu(int x, int y)
{
	s2a_send_message(S2A_KEY_X, x);
	s2a_send_message(S2A_KEY_Y, y);
	s2a_send_message(S2A_FCT_XY_RELATIF);
	s2a_send_message(S2A_FCT_MODE_TENDU);
	start();
}

void test_asser_xy_absolu_tendu(int x, int y)
{
	s2a_send_message(S2A_KEY_X, x);
	s2a_send_message(S2A_KEY_Y, y);
	s2a_send_message(S2A_FCT_XY_ABSOLU);
	s2a_send_message(S2A_FCT_MODE_TENDU);
	start();
}

void test_asser_chemin(s_liste chemin)
{
	set_trajectoire_chemin(chemin);
	start();
}

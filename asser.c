#include "asser.h"
#include "PID.h"
#include "hardware.h"
#include "trajectoire.h"
#include "odometrie.h"
#include "communication.h"
#include "reglages.h"
#include "SDL/affichage.h" //à virer
#include <stdio.h> //à virer

void asser()
{
	//init
	init_odometrie();
	init_alpha_delta_voulu();
	int erreur_delta_preced=0;
	int erreur_delta_sum=0;
	int erreur_alpha_preced=0;
	int erreur_alpha_sum=0;
	int reponse_delta_preced=0;
	int reponse_alpha_preced=0;
	set_new_alpha_delta(00, 1800); //à effacer
	//set_new_xy_relatif(1800,0);//à effacer

	while(!sdl_manage_events())
	{
		//synchronisation à une fréquence régulière
		while(doit_attendre());

		//calcul de l'erreur en delta et alpha
		int erreur_delta=get_delta_voulu()-get_delta_actuel();
		int erreur_alpha=get_alpha_voulu()-get_alpha_actuel();
		printf("e_a:%d e_D:%d ",erreur_alpha,erreur_delta);

		//calcul de la réponse des PIDs
		int reponse_delta=PID_lineique(erreur_delta,erreur_delta_preced,erreur_delta_sum);
		int reponse_alpha=PID_angulaire(erreur_alpha,erreur_alpha_preced,erreur_alpha_sum);
		printf("r_a:%d r_D:%d ",reponse_alpha,reponse_delta);

		//mise à jour des variable d'intégration et de dérivationS
		erreur_delta_preced=erreur_delta;
		erreur_alpha_preced=erreur_alpha;
		erreur_delta_sum+=erreur_delta; //employer une autre méthode pour éviter un overflow
		erreur_alpha_sum+=erreur_alpha; //employer une autre méthode pour éviter un overflow

		//vérification des réponses sorties des PIDs (pas trop grand ni trop petit)
		verifie_reponse(&reponse_delta,reponse_delta_preced);
		verifie_reponse(&reponse_alpha,reponse_alpha_preced);
		printf("rv_a:%d rv_D:%d\n",reponse_alpha,reponse_delta);

		//on converti les réponses des PIDs en commande pour les moteurs
		int commande_moteur_D=reponse_delta+DEMI_ENTRAXE*reponse_alpha;
		int commande_moteur_G=reponse_delta-DEMI_ENTRAXE*reponse_alpha;
		printf("com_D:%d com_G:%d\n",commande_moteur_D,commande_moteur_G);

		//on regarde si on est pas arrivé à bon port
		//et si on peut s'arreter sans risquer de tomber
		if (asser_done(erreur_delta,erreur_alpha)
			&& arret_ok(commande_moteur_D,commande_moteur_G))
		{
			//on réinitialise les valeurs
			erreur_delta_preced=0;
			erreur_alpha_preced=0;
			erreur_delta_sum=0;
			erreur_alpha_sum=0;
			reponse_delta_preced=0;
			reponse_alpha_preced=0;
			commande_moteur_D=0;
			commande_moteur_G=0;
			set_delta_actuel(0);
			set_alpha_actuel(0);
			set_delta_voulu(0);
			set_alpha_voulu(0);
			//or fait savoir que la position est atteinte
			send_position_atteinte(); //ajouter anti-spam (ici on envoie sans arret)
		}

		//on converti les commandes en PWM et direction pour les ponts en H
		int PWM_moteur_D=convert2PWM(commande_moteur_D);
		int PWM_moteur_G=convert2PWM(commande_moteur_G);
		printf("PWM_D:%d PWM_G:%d\n",PWM_moteur_D,PWM_moteur_G);

		//on applique les PWM et signaux de direction
		set_PWM_moteur_D(PWM_moteur_D);
		set_PWM_moteur_G(PWM_moteur_G);

		//on actualise la position actuelle du robot (via les roues codeuses)
		actualise_position();

		//on envoie notre position au PC
		send_position_xbee();
	}
}

void verifie_reponse(int * reponse,int reponse_preced)
{
	//gestion de l'acceleration max et de la deceleration max
	if (*reponse-reponse_preced>MAX_ACCELERATION)
	{
		*reponse=reponse_preced+MAX_ACCELERATION;
	}
	else if (*reponse-reponse_preced<-MAX_DECELERATION)
	{
		*reponse=reponse_preced-MAX_DECELERATION;
	}

	//gestion de la vitesse max
	if (*reponse>MAX_VITESSE) 
	{
		*reponse=MAX_VITESSE;
	}
	else if (*reponse<-MAX_VITESSE)
	{
		*reponse=-MAX_VITESSE;
	}

	//gestion de la vitesse min
	else if (0<*reponse && *reponse<MIN_VITESSE) 
	{
		*reponse=MIN_VITESSE;
	}
	else if (-MIN_VITESSE<*reponse && *reponse<0)
	{
		*reponse=-MIN_VITESSE;
	}
}

int asser_done(int erreur_delta, int erreur_alpha)
{
	if (abs(erreur_delta)<PRECISION_DELTA && abs(erreur_alpha)<PRECISION_ALPHA)
	{
		return 1;
	}
	return 0;
}

int arret_ok(int commande_moteur_D, int commande_moteur_G)
{
	if(abs(commande_moteur_G)<VIT_MAX_ARRET && abs(commande_moteur_D)<VIT_MAX_ARRET)
	{
		return 1;
	}
	return 0;
}

int convert2PWM(int commande)
{
	return (1000*commande/MAX_VITESSE*PWM_MAX);
}

int abs(int entier_relatif)
{
	//muhahaha j'aime ne pas faire comme tout le monde
	int entier_naturel=entier_relatif;
	entier_naturel-=2*entier_relatif*(entier_relatif<0);
	return entier_naturel;
}
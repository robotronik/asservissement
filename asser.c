#include "asser.h"
#include "PID.h"
#include "hardware.h"
#include "trajectoire.h"
#include "odometrie.h"
#include "communication.h"
#include "reglages.h"
#include "debug/affichage.h" //à virer
#include <stdio.h> //à virer
#define AFFICHAGE_DEBUG 0

void asser()
{
	//init
	s_erreur erreur_alpha={0,0,0};
	s_erreur erreur_delta={0,0,0};
	long int reponse_delta_preced=0;
	long int reponse_alpha_preced=0;
	//set_new_xy_absolu(1000, 1000); //à effacer
	//set_new_xy_relatif(1000,1000);//à effacer
	//set_new_alpha_delta(0*-3142/4,-1000);//à effacer

	while(!sdl_manage_events())
	{
		//synchronisation à une fréquence régulière
		while(doit_attendre());

		//mise à jour des erreurs en delta et alpha
		update_erreurs(&erreur_delta, &erreur_alpha);
		if (AFFICHAGE_DEBUG == 1)
        	printf("e_a:%i e_D:%i ",erreur_alpha.actuelle,erreur_delta.actuelle);

		//calcul des réponses provenant des PIDs
		long int reponse_delta=PID_lineique(erreur_delta);
		long int reponse_alpha=PID_angulaire(erreur_alpha);
		if (AFFICHAGE_DEBUG == 1)
        	printf("r_a:%li r_D:%li ",reponse_alpha,reponse_delta);

		//on écrête les réponses en sortie des PIDs si trop grand ou trop petit
		//TODO : à effectuer sur les commandes moteurs plutot que sur les reponses
		ecretage_reponse(&reponse_delta,reponse_delta_preced);
		ecretage_reponse(&reponse_alpha,reponse_alpha_preced);
		if (AFFICHAGE_DEBUG == 1)
        	printf("recr_a:%li recr_D:%li\n",reponse_alpha,reponse_delta);

		//on actualise les valeurs précédantes
		reponse_delta_preced=reponse_delta;
		reponse_alpha_preced=reponse_alpha;

		//on convertit les réponses des PIDs en commandes pour les moteurs
		//multiplier par DEMI_ENTRAXE n'est pas forcement utile car il peut se retrouver dans les coeffs de PID angulaire
		long int commande_moteur_D=reponse_delta+DEMI_ENTRAXE*reponse_alpha;
		long int commande_moteur_G=reponse_delta-DEMI_ENTRAXE*reponse_alpha;
		if (AFFICHAGE_DEBUG == 1)
        	printf("com_D:%li com_G:%li\n",commande_moteur_D,commande_moteur_G);

		//on regarde si on est pas arrivé à bon port
		//et si on peut s'arreter sans risquer de tomber
		if (asser_done(erreur_delta.actuelle,erreur_alpha.actuelle)
			&& arret_ok(commande_moteur_D,commande_moteur_G))
		{
			//on réinitialise les valeurs
			erreur_delta.preced=0;
			erreur_alpha.preced=0;
			erreur_delta.sum=0;
			erreur_alpha.sum=0;
			reponse_delta_preced=0;
			reponse_alpha_preced=0;
			commande_moteur_D=0;
			commande_moteur_G=0;
			set_delta_actuel(0);
			set_alpha_actuel(0);
			set_delta_voulu(0);
			set_alpha_voulu(0);
			//on fait savoir que la position est atteinte
			send_position_atteinte(); //ajouter anti-spam (ici on envoie sans arret)
		}

		//on convertit les commandes en PWM et direction pour les ponts en H
		int PWM_moteur_D=convert2PWM(commande_moteur_D);
		int PWM_moteur_G=convert2PWM(commande_moteur_G);
		if (AFFICHAGE_DEBUG == 1)
        	printf("PWM_D:%i PWM_G:%i\n",PWM_moteur_D,PWM_moteur_G);

		//on applique les PWM et signaux de direction
		set_PWM_moteur_D(PWM_moteur_D);
		set_PWM_moteur_G(PWM_moteur_G);

		//on actualise la position actuelle du robot (via les roues codeuses)
		actualise_position();

		//on envoie notre position au PC
		send_position_xbee();

		//on update la consigne de position
		update_consigne();
	}
}

void update_erreurs(s_erreur * erreur_delta, s_erreur * erreur_alpha)
{
	//mise à jour de la valeur précédante
	erreur_delta->preced=erreur_delta->actuelle;
	erreur_alpha->preced=erreur_alpha->actuelle;

	//mise à jour de la valeur de l'intégrale temporelle de l'erreur
		//utiliser une intégration par méthode des trapèzes à la place ?
	//TODO : employer une autre méthode pour éviter un overflow -> Antiwindup ?
	erreur_delta->sum += erreur_delta->actuelle;
	erreur_alpha->sum += erreur_alpha->actuelle;

	//calcul de l'erreur actuelle en delta et alpha
	erreur_delta->actuelle = get_delta_voulu()-get_delta_actuel();
	erreur_alpha->actuelle = get_alpha_voulu()-get_alpha_actuel();
}

void ecretage_reponse(long int * reponse,long int reponse_preced)
{
	//TODO : à tester avec MIN_VITESSE différent de 0
	//gestion de l'acceleration max et de la deceleration max
	if (abs(*reponse)-abs(reponse_preced)>MAX_ACCELERATION)
	{
		//avance
		if (*reponse>0)
		{
			*reponse=reponse_preced+MAX_ACCELERATION;
		}
		//recule
		else if (*reponse<0)
		{
			*reponse=reponse_preced-MAX_ACCELERATION;
		}
	}
	else if (abs(*reponse)-abs(reponse_preced)<-MAX_DECELERATION)
	{
		//avance
		if (*reponse>0)
		{
			*reponse=reponse_preced-MAX_DECELERATION;
		}
		//recule
		else if (*reponse<0)
		{
			*reponse=reponse_preced+MAX_DECELERATION;
		}
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

int arret_ok(long int commande_moteur_D, long int commande_moteur_G)
{
	if(abs(commande_moteur_G)<VIT_MAX_ARRET && abs(commande_moteur_D)<VIT_MAX_ARRET)
	{
		return 1;
	}
	return 0;
}

int convert2PWM(long int commande)
{
	return (int)(1000*commande/MAX_VITESSE*PWM_MAX);
}

/*long int abs(long int entier_relatif)
{
	//muhahaha j'aime ne pas faire comme tout le monde
	long int entier_naturel=entier_relatif;
	entier_naturel-=2*entier_relatif*(entier_relatif<0);
	return entier_naturel;
}*/

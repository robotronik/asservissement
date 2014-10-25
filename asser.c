#include "asser.h"
#include "PID.h"
#include "meca.h"
#include "trajectoire.h"
#include "odometrie.h"

void asser()
{
	//init
	init_odometrie();
	int erreur_delta_preced=0;
	int erreur_delta_sum=0;
	int erreur_alpha_preced=0;
	int erreur_alpha_sum=0;
	int reponse_delta_preced=0;
	int reponse_alpha_preced=0;

	while(1)
	{
		pause(); //à revoir pour utiliser le timer à la place

		int x_voulu=get_x_voulu(); //à revoir pour utiliser les interruptions
		int y_voulu=get_y_voulu();
		int a_voulu=get_a_voulu();

		//calcul de delta et alpha
		int delta_voulu=calcul_delta(x_voulu,y_voulu,a_voulu);
		int alpha_voulu=calcul_alpha(x_voulu,y_voulu,a_voulu);

		//calcul de l'erreur en delta et alpha
		int erreur_delta=delta_voulu-get_delta_actuel();
		int erreur_alpha=alpha_voulu-get_alpha_actuel();

		//calcul de la réponse des PIDs
		int reponse_delta=PID_lineique(erreur_delta,erreur_delta_preced,erreur_delta_sum);
		int reponse_alpha=PID_angulaire(erreur_alpha,erreur_alpha_preced,erreur_alpha_sum);

		//mise à jour des variable d'intégration et de dérivationS
		erreur_delta_preced=erreur_delta;
		erreur_alpha_preced=erreur_alpha;
		erreur_delta_sum+=erreur_delta; //employer une autre méthode pour éviter un overflow
		erreur_alpha_sum+=erreur_alpha; //employer une autre méthode pour éviter un overflow

		//vérification des réponses sorties des PIDs (pas trop grand ni trop petit)
		valide(&reponse_delta,reponse_delta_preced);
		valide(&reponse_alpha,reponse_alpha_preced);

		//on converti les réponses des PIDs en commande pour les moteurs
		int commande_moteur_D=reponse_delta+DEMI_ENTRAXE*reponse_alpha;
		int commande_moteur_G=reponse_delta-DEMI_ENTRAXE*reponse_alpha;

		//on regarde si on est pas arrivé à bon port
		if (erreur_delta<PRECISION_DELTA && erreur_alpha<PRECISION_ALPHA)
		{
			if(commande_moteur_G<VIT_MAX_ARRET && commande_moteur_D<VIT_MAX_ARRET)
			{
				reponse_delta_preced=0;
				reponse_alpha_preced=0;
				commande_moteur_D=0;
				commande_moteur_G=0;
				send_position_atteinte();
			}
		}

		//on converti les commandes en PWM et direction pour les ponts en H
		int PWM_moteur_D=convert2PWM(commande_moteur_D);
		int PWM_moteur_G=convert2PWM(commande_moteur_G);

		//on applique les PWM et signaux de direction
		set_PWM_moteur_D(PWM_moteur_D);
		set_PWM_moteur_G(PWM_moteur_G);

		//on récupère le nombre de tick éffectuer sur les roues codeuses
		int nbr_tick_D=get_nbr_tick_D();
		int nbr_tick_G=get_nbr_tick_G();

		//on actualise la position actuelle du robot
		actualise_position(nbr_tick_D,nbr_tick_G);
	}
}

void valide(int * reponse,int reponse_preced)
{
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
	else if (MIN_VITESSE<*reponse && *reponse<0)
	{
		*reponse=-MIN_VITESSE;
	}

	//gestion de l'acceleration max et de la deceleration max
	if (*reponse-reponse_preced>MAX_ACCELERATION)
	{
		*reponse=*reponse+MAX_ACCELERATION;
	}
	else if (*reponse-reponse_preced<-MAX_DECELERATION)
	{
		*reponse=*reponse-MAX_DECELERATION;
	}

}

int conver2PWM(int* commande,int reponse_preced)
{

}
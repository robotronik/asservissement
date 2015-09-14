#include <stdbool.h>

#include "reglages.h"

#include "../hardware/debug.h"
#include "../common_code/communication/text_emission.h"
#include "../common_code/communication/a2s.h"

#include "PID.h"
#include "hardware.h"
#include "trajectoire.h"
#include "odometrie.h"

#include "asser.h"

void update_erreurs(s_consigne consigne);
void mise_echelle(s_vitesse * vitesse);
void ecretage_acceleration(long int * reponse,long int reponse_preced);
void ecretage_deceleration(long int * reponse,long int reponse_preced);
void ecretage_vitesse(long int * reponse);
int asser_done(int erreur_delta, int erreur_alpha);
int arret_ok(long int commande_moteur_D,long int commande_moteur_G);

s_vitesse asser_position(s_consigne consigne);
void post_traitement_vitesse(s_vitesse * vitesse);
void gestion_position_atteinte(s_vitesse * vitesse);
s_pwm asser_vitesse(s_vitesse vitesse);
void post_traitement_pwm(s_pwm * pwm);
void applique_pwm(s_pwm pwm);

static s_erreur erreur_alpha;
static s_erreur erreur_delta;
static long int commande_moteur_D_preced;
static long int commande_moteur_G_preced;

void asser(s_consigne consigne)
{
	//synchronisation à une fréquence régulière
	attente_synchro();
	//asservissement en position
	s_vitesse vitesse=asser_position(consigne);
	//traitement de la réponse de l'asser en consigne
	post_traitement_vitesse(&vitesse);
	//si la position est atteinte
	gestion_position_atteinte(&vitesse);
	//TODO : nettoyer
	//actualisation des valeurs précédantes
	commande_moteur_D_preced=vitesse.moteur_D;
	commande_moteur_G_preced=vitesse.moteur_G;
	//assevissement en vitesse
	s_pwm pwm=asser_vitesse(vitesse);
	//traitement des pwm
	post_traitement_pwm(&pwm);
	//application des pwm sur les moteurs
	applique_pwm(pwm);
}

s_vitesse asser_position(s_consigne consigne)
{
	//mise à jour des erreurs en delta et alpha
	update_erreurs(consigne);
	debug(_VERBOSE_, "e_a:%i e_D:%i ",erreur_alpha.actuelle,erreur_delta.actuelle);

	//calcul des réponses provenant des PIDs
	long int reponse_delta=PID_lineique(erreur_delta);
	long int reponse_alpha=PID_angulaire(erreur_alpha);
	debug(_VERBOSE_, "r_a:%li r_D:%li ",reponse_alpha,reponse_delta);

	//on convertit les réponses des PIDs en vitesses pour les moteurs
	s_vitesse vitesse;
	vitesse.moteur_D=reponse_delta+reponse_alpha;
	vitesse.moteur_G=reponse_delta-reponse_alpha;
	debug(_VERBOSE_, "com_D:%li com_G:%li\n",vitesse.moteur_D,vitesse.moteur_G);

	return vitesse;
}

void post_traitement_vitesse(s_vitesse * vitesse)
{
    //correction eventuelle des commandes
    vitesse->moteur_D*=COEFF_MOTEUR_D;
    vitesse->moteur_G*=COEFF_MOTEUR_G;

	//propotions correctes pour les commandes
#   if PIC_BUILD
		//mise_echelle(&commande_moteur_D,&commande_moteur_G);
#   else
    	mise_echelle(vitesse);
#   endif

	//écretage (si trop forte acceleration/décélérantion)
	//TODO : nettoyer
	ecretage(&(vitesse->moteur_D),commande_moteur_D_preced);
	ecretage(&(vitesse->moteur_G),commande_moteur_G_preced);
}

void gestion_position_atteinte(s_vitesse * vitesse)
{
	//on regarde si on est pas arrivé à bon port
	//et si on peut s'arreter sans risquer de tomber
    static bool deja_notifie = false;
	if (asser_done(erreur_delta.actuelle,erreur_alpha.actuelle)
		&& arret_ok(commande_moteur_D_preced,commande_moteur_G_preced))
	{
		//on réinitialise les valeurs
		erreur_delta.preced=0;
		erreur_alpha.preced=0;
		erreur_delta.sum=0;
		erreur_alpha.sum=0;
		//il n'est plus utile que les moteurs tournent
		vitesse->moteur_D=0;
		vitesse->moteur_G=0;
		//on fait savoir que la position est atteinte
		if (!deja_notifie) {
			//send_cmd(a2s_keys[A2S_CMD_DONE]); //ajouter anti-spam (ici on envoie sans arret)
                    allumer_del();
			deja_notifie = true;
		}
	}
	else if (asser_done(erreur_delta.actuelle,erreur_alpha.actuelle))
	{
		deja_notifie = false;
		debug(_ERROR_, "atteint mais peu pas s'arreter");
	} else {
		deja_notifie = false;
                eteindre_del();
	}
}

s_pwm asser_vitesse(s_vitesse vitesse)
{
	//TODO : remplacer par un vrai asser
	//on convertit les commandes en PWM et direction pour les ponts en H
	s_pwm pwm;
	pwm.moteur_D=convert2PWM(vitesse.moteur_D);
	pwm.moteur_G=convert2PWM(vitesse.moteur_G);

	debug(_VERBOSE_, "PWM_D:%i PWM_G:%i\n",pwm.moteur_D,pwm.moteur_G);

	return pwm;
}

void post_traitement_pwm(s_pwm * pwm)
{

}

void applique_pwm(s_pwm pwm)
{
	//on applique les PWM et signaux de direction
	set_PWM_moteur_D(pwm.moteur_D);
	set_PWM_moteur_G(pwm.moteur_G);
}

void init_asser()
{
	erreur_alpha.preced=0;
	erreur_alpha.actuelle=0;
	erreur_alpha.sum=0;
	erreur_delta.preced=0;
	erreur_delta.actuelle=0;
	erreur_delta.sum=0;
	commande_moteur_D_preced=0;
	commande_moteur_G_preced=0;
}

void update_erreurs(s_consigne consigne)
{
	//mise à jour de la valeur précédante
	erreur_delta.preced=erreur_delta.actuelle;
	erreur_alpha.preced=erreur_alpha.actuelle;

	//mise à jour de la valeur de l'intégrale temporelle de l'erreur
	//TODO : utiliser une méthode pour éviter un overflow (Antiwindup ?)
	erreur_delta.sum += erreur_delta.actuelle;
	erreur_alpha.sum += erreur_alpha.actuelle;

	//calcul de l'erreur actuelle en delta et alpha
	erreur_delta.actuelle = (int) (consigne.delta-get_delta_actuel());
	erreur_alpha.actuelle = (int) (consigne.alpha-get_alpha_actuel());
}

void mise_echelle(s_vitesse * vitesse)
{
	long int vitesse_D=vitesse->moteur_D;
	long int vitesse_G=vitesse->moteur_G;

	if(abs(vitesse_D)>abs(vitesse_G) && abs(vitesse_D)>MAX_VITESSE)
	{
		vitesse_G=vitesse_G*MAX_VITESSE/abs(vitesse_D);
		vitesse_D=sgn(vitesse_D)*MAX_VITESSE;
	}
	if(abs(vitesse_G)>abs(vitesse_D) && abs(vitesse_G)>MAX_VITESSE)
	{
		vitesse_D=vitesse_D*MAX_VITESSE/abs(vitesse_G);
		vitesse_G=sgn(vitesse_G)*MAX_VITESSE;
	}

	vitesse->moteur_D=vitesse_D;
	vitesse->moteur_G=vitesse_G;
}

void ecretage(long int * reponse,long int reponse_preced)
{
	//si on accelere trop
	ecretage_acceleration(reponse, reponse_preced);

	//si on decelere trop
	ecretage_deceleration(reponse, reponse_preced);

	//si reculait mais que maintenant on veut avancer
	if(reponse_preced<0 && *reponse>0)
	{
		*reponse=reponse_preced+MAX_DECELERATION;
		if(*reponse>-MIN_VITESSE)
		{
			//on force le passage à 0 pour éviter un à-coup sur les moteurs
			*reponse=0;
		}
	}

	//si on avancait mais que maintenant on veut reculer
	if(reponse_preced>0 && *reponse<0)
	{
		*reponse=reponse_preced-MAX_DECELERATION;
		if(*reponse<MIN_VITESSE)
		{
			//on force le passage à 0 pour éviter un à-coup sur les moteurs
			*reponse=0;
		}
	}

	//si on va trop vite ou pas assez
	//devenu inutile normalement du fait de mise_echelle()
	ecretage_vitesse(reponse);

}

void ecretage_acceleration(long int * reponse,long int reponse_preced)
{
	if(abs(*reponse)-abs(reponse_preced)>MAX_ACCELERATION)
	{
		//si on avance
		if(reponse_preced>=0 && *reponse>=0)
		{
			*reponse=reponse_preced+MAX_ACCELERATION;
		}
		//si on recule
		if(reponse_preced<=0 && *reponse<=0)
		{
			*reponse=reponse_preced-MAX_ACCELERATION;
		}
	}
}

void ecretage_deceleration(long int * reponse,long int reponse_preced)
{
	if(abs(*reponse)-abs(reponse_preced)<-MAX_DECELERATION)
	{
		//si on avance
		if(reponse_preced>=0 && *reponse>=0)
		{
			*reponse=reponse_preced-MAX_DECELERATION;
		}
		//si on recule
		if(reponse_preced<=0 && *reponse<=0)
		{
			*reponse=reponse_preced+MAX_DECELERATION;
		}
	}
}

void ecretage_vitesse(long int * reponse)
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

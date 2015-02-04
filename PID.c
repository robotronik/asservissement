#include "PID.h"
#include "reglages.h"
#include "asser.h"

long int PID_lineique(s_erreur erreur_delta)
{
	long int commande;

	//composante proportionelle
	commande=(long int) KP_DELTA*erreur_delta.actuelle;
	//composante intégrale
	commande+=(long int) KI_DELTA*erreur_delta.sum;
	//composante dérivée
	commande+=(long int) KD_DELTA*(erreur_delta.actuelle-erreur_delta.preced);

	return commande;
}

long int PID_angulaire(s_erreur erreur_alpha)
{
	long int commande;
	
	//composante proportionelle
	commande=(long int) KP_ALPHA*erreur_alpha.actuelle;
	//composante intégrale
	commande+=(long int) KI_ALPHA*erreur_alpha.sum;
	//composante dérivée
	commande+=(long int) KD_ALPHA*(erreur_alpha.actuelle-erreur_alpha.preced);

	return commande;
}
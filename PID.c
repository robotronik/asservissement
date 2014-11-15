#include "PID.h"
#include "reglages.h"
#include "asser.h"

//TODO : rajouter un Antiwindup ??
long int PID_lineique(s_erreur erreur_delta)
{
//erreur_sum intégration par somme ou méthode des trapèzes ?
	long int commande;

	commande=(long) KP_DELTA*erreur_delta.actuelle; //composante proportionelle
	commande+=(long) KI_DELTA*erreur_delta.sum; //composante intégrale
	commande+=(long) KD_DELTA*(erreur_delta.actuelle-erreur_delta.preced); //composante dérivée

	return commande;
}

long int PID_angulaire(s_erreur erreur_alpha)
{
//erreur_sum intégration par somme ou méthode des trapèzes ?
	long int commande;

	commande=(long) KP_ALPHA*erreur_alpha.actuelle; //composante proportionelle
	commande+=(long) KI_ALPHA*erreur_alpha.sum; //composante intégrale
	commande+=(long) KD_ALPHA*(erreur_alpha.actuelle-erreur_alpha.preced); //composante dérivée

	return commande;
}
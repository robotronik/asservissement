#include "PID.h"
#include "reglages.h"

long int PID_lineique(int erreur, int erreur_preced, int erreur_sum)
{
//erreur_sum intégration par somme ou méthode des trapèzes ?
	long int commande;

	commande=(long) KP_DELTA*erreur; //composante proportionelle
	commande+=(long) KI_DELTA*erreur_sum; //composante intégrale
	commande+=(long) KD_DELTA*(erreur-erreur_preced); //composante dérivée

	return commande;
}

long int PID_angulaire(int erreur, int erreur_preced, int erreur_sum)
{
//erreur_sum intégration par somme ou méthode des trapèzes ?
	long int commande;

	commande=(long) KP_ALPHA*erreur; //composante proportionelle
	commande+=(long) KI_ALPHA*erreur_sum; //composante intégrale
	commande+=(long) KD_ALPHA*(erreur-erreur_preced); //composante dérivée

	return commande;
}
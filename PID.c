#include "PID.h"
#include "reglages.h"

long int PID_lineique(int erreur, int erreur_preced, int erreur_sum)
{
//erreur_sum intégration par somme ou méthode des trapèzes ?
	long int commande;

	commande= KP_DELTA*erreur; //composante proportionelle
	commande+= KI_DELTA*erreur_sum; //composante intégrale
	commande+= KD_DELTA*(erreur-erreur_preced); //composante dérivée

	return commande;
}

long int PID_angulaire(int erreur, int erreur_preced, int erreur_sum)
{
//erreur_sum intégration par somme ou méthode des trapèzes ?
	long int commande;

	commande= KP_ALPHA*erreur; //composante proportionelle
	commande+= KI_ALPHA*erreur_sum; //composante intégrale
	commande+= KD_ALPHA*(erreur-erreur_preced); //composante dérivée

	return commande;
}
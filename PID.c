#include "PID.h"

int PID_lineique(int erreur, int erreur_preced, int erreur_sum)
{
//erreur_sum intégration par somme ou méthode des trapèzes ?
	int commande;

	commande= Kp*erreur; //composante proportionelle
	commande+= Ki*erreur_sum; //composante intégrale
	commande+= Kd*(erreur-erreur_preced); //composante dérivée

	return commande;
}

int PID_angulaire(int erreur, int erreur_preced, int erreur_sum)
{
//erreur_sum intégration par somme ou méthode des trapèzes ?
	int commande;

	commande= Kp*erreur; //composante proportionelle
	commande+= Ki*erreur_sum; //composante intégrale
	commande+= Kd*(erreur-erreur_preced); //composante dérivée

	return commande;
}
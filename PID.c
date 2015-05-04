#include "PID.h"
#if PIC_BUILD
#	if   GROS
#		include "reglages_gros.h"
#	elif PETIT
#		include "reglages_petit.h"
#	endif
#else
#	include "reglages_PC.h"
#endif
#include "asser.h"

static int kp_delta=KP_DELTA;
static int kd_delta=KD_DELTA;
static int kp_alpha=KP_ALPHA;
static int kd_alpha=KD_ALPHA;

long int PID_lineique(s_erreur erreur_delta)
{
	long int commande;

	//composante proportionelle
	commande=(long int) kp_delta*erreur_delta.actuelle;
	//composante intégrale
	commande+=(long int) KI_DELTA*erreur_delta.sum;
	//composante dérivée
	commande+=(long int) kd_delta*(erreur_delta.actuelle-erreur_delta.preced);

	return commande;
}

long int PID_angulaire(s_erreur erreur_alpha)
{
	long int commande;
	
	//composante proportionelle
	commande=(long int) kp_alpha*erreur_alpha.actuelle;
	//composante intégrale
	commande+=(long int) KI_ALPHA*erreur_alpha.sum;
	//composante dérivée
	commande+=(long int) kp_alpha*(erreur_alpha.actuelle-erreur_alpha.preced);

	return commande;
}

void set_kp_delta(int new_kp_delta)
{
    kp_delta=new_kp_delta;
}

void set_kd_delta(int new_kd_delta)
{
    kd_delta=new_kd_delta;
}

void set_kp_alpha(int new_kp_alpha)
{
    kp_alpha=new_kp_alpha;
}

void set_kd_alpha(int new_kd_alpha)
{
    kd_alpha=new_kd_alpha;
}
#include "PID.h"
#include "reglages.h"
#include "asser.h"

static int kp_delta=KP_DELTA;
static int kd_delta=KD_DELTA;
static int kp_alpha=KP_ALPHA;
static int kd_alpha=KD_ALPHA;
static int kp_roue_D=KP_ROUE_D;
static int ki_roue_D=KI_ROUE_D;
static int kd_roue_D=KD_ROUE_D;
static int kp_roue_G=KP_ROUE_G;
static int ki_roue_G=KI_ROUE_G;
static int kd_roue_G=KD_ROUE_G;

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
	commande+=(long int) kd_alpha*(erreur_alpha.actuelle-erreur_alpha.preced);

	return commande;
}

long int PID_vitesse_roue_D(s_erreur erreur_vit_roue_D)
{
	long int commande;
	
	//composante proportionelle
	commande=(long int) kp_roue_D*erreur_vit_roue_D.actuelle;
	//composante intégrale
	commande+=(long int) ki_roue_D*erreur_vit_roue_D.sum;
	//composante dérivée
	commande+=(long int) kd_roue_D*(erreur_vit_roue_D.actuelle-erreur_vit_roue_D.preced);

	return commande;
}

long int PID_vitesse_roue_G(s_erreur erreur_vit_roue_G)
{
	long int commande;
	
	//composante proportionelle
	commande=(long int) kp_roue_G*erreur_vit_roue_G.actuelle;
	//composante intégrale
	commande+=(long int) ki_roue_G*erreur_vit_roue_G.sum;
	//composante dérivée
	commande+=(long int) kd_roue_G*(erreur_vit_roue_G.actuelle-erreur_vit_roue_G.preced);

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
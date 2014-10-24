/*-----------------------------------------------------*
 * Calcul des pid et asservissement                    *
 *-----------------------------------------------------*/



#ifndef _ASSER_H
#define _ASSER_H

#include "main.h"

// 1 tck = distance parcourue par une roue codeuse pour que le codeur s'incrémente




//////////////////////////////////////////
#define PI 3.1415926535897932384626433832795
#define DEUX_PI 6.283185307179586476925286766559
#define TICK_TOUR 17145.1058519337//16308,188814593993325917686318131//16265.1810//16220.//16330. //Nombre de tick pour faire un tour
#define TICK_ROUE 1440 //Nombre de cran/tick par tour de roue

#define COEFF 100 //12
#define AD_MAX_SPEED 39990
//#define AD_MIN_SPEED  8000//10000

#define VIT_MAX 15000

#define TICKS_TO_MM 0.04528165 // Non divisé par 2 : 0,09056330 // mm/tick
#define MM_TO_TICKS 22.084 // tick/mm
//#define ENTRE_AXE_TICKS 2627.2211093803600792287934402566// 1021.0176124166828025003590995658 // en mm 325


#define SEUIL_DERAP 1.1

#define CORFUGE -1.2 //RCVA : 1.2
#define COEFF_DIFF_ROUE 1.0072294372

#define AD_abs(valeur) ((valeur) < 0 ? -(valeur) : (valeur) )
#define AD_max(a,b) ((AD_abs(a) > AD_abs(b)) ? AD_abs(a) : AD_abs(b))


long int deriv_erreurs(long int erreurs[4]);
long int rampe(long int vcommande, long int prec);
long int echelle(long int c);

void AD_motors_set_speed(short speedL, short speedR);
void majXYAngle(double dTheta, double dDelta);

long int moyenne_erreurs( long int erreurs[4]);
void maj_erreurs( long int erreurs[4],long int erreur);

void setGros();
void setPrecis();



struct AD_K 
{
	/*double delta;
	double alpha;*/
	long int delta;
	long int alpha;
};
typedef struct AD_K AD_K; 	

//////////////////////////////////////////

void asser();

#endif

/*/!\ si le robot commence à effectuer le mouvement et revient mystérieusement
en arrière c'est peut-être que vous avez un peu exagéré sur les valeurs des 
coefficients des PID et qu'il y a overflow*/

//PID lineique
#define KP_DELTA 200//constante de proportionalité
#define KI_DELTA 11400//composante d'intégration
#define KD_DELTA 1050000//composante de dérivation
//PID angulaire
#define KP_ALPHA 2000//constante de proportionalité
#define KI_ALPHA 1//composante d'intégration
#define KD_ALPHA 200//composante de dérivation

//contraintes mecaniques
//#define ENTRAXE 10//mm
#define DEMI_ENTRAXE 5 //mm ENTRAXE divisé par 2
#define MAX_VITESSE 40 //vitesse maximale du robot
#define MIN_VITESSE 8 //vitesse minimale du robot pour qu'il commence à avancer
#define MAX_ACCELERATION 2 //acceleration maximale
#define MAX_DECELERATION 1 //deceleration maximale (valeur absolue)
#define VIT_MAX_ARRET 15000 //vitesse maximale à laquelle on peut s'arreter sans tomber
#define PWM_MAX 1 //PWM maximal, à cette valeur le robot est à sa vitesse maximale admissible
//"vitesse" correspond à la commande pour chaque moteur avant convertion en PWM
//"acceleration" et "deceleration" : différence entre deux valeurs de "vitesses" consécutives

//precision
#define PRECISION_DELTA 1 //precision souhaitée pour delta (en mm)
#define PRECISION_ALPHA 10 //precision souhaitée pour alpha (en milli radian)

//calibrage
#define TICK_PAR_MM 22 // nombre de ticks par mm (tick/mm)
#define TICK_PAR_TOUR 17145 //nombre de ticks par tour complet du robot sur lui-même (tick/tour)
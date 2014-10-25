//PID lineique
#define KD_DELTA 1//constante de proportionalité
#define KI_DELTA 1//composante d'intégration
#define KD_DELTA 1//composante de dérivation
//PID angulaire
#define KD_ALPHA 1//constante de proportionalité
#define KI_ALPHA 1//composante d'intégration
#define KD_ALPHA 1//composante de dérivation

//contraintes mecaniques
#define ENTRAXE 10//mm
#define DEMI_ENTRAXE 5 //mm ENTRAXE divisé par 2
#define MAX_VITESSE 40000 //vitesse maximale du robot
#define MIN_VITESSE 8000 //vitesse minimale du robot pour qu'il commence à avancer
#define MAX_ACCELERATION 20 //acceleration maximale
#define MAX_DECELERATION 20 //deceleration minimale
#define VIT_MAX_ARRET 15000 //vitesse maximale à laquelle on peut s'arreter sans tomber
#define PWM_MAX 1 //PWM maximal, à cette valeur le robot est à sa vitesse maximale admissible
//"vitesse" correspond à la commande pour chaque moteur avant convertion en PWM
//"acceleration" et "deceleration" : différence entre deux valeurs de "vitesses" consécutives

//precision
#define PRECISION_DELTA 1 //precision souhaitée pour delta (en mm)
#define PRECISION_ALPHA 1 //precision souhaitée pour alpha (en rad)
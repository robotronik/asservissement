//PID lineique
#define KD_DELTA 1//constante de proportionalité
#define KI_DELTA 1//composante d'intégration
#define KD_DELTA 1//composante de dérivation
//PID angulaire
#define KD_ALPHA 1//constante de proportionalité
#define KI_ALPHA 1//composante d'intégration
#define KD_ALPHA 1//composante de dérivation

//contraintes mecaniques
#define ENTRAXE //mm
#define DEMI_ENTRAXE 5 //mm ENTRAXE divisé par 2
#define MAX_VITESSE 40000 //vitesse maximale du robot
#define MIN_VITESSE 8000 //vitesse minimale du robot pour qu'il commence à avancer
#define MAX_ACCELERATION 20 //acceleration maximale
#define MAX_DECELERATION 20 //deceleration minimale
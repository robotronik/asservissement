#ifndef REGLAGES_H
#define REGLAGES_H

//PID lineique
#define KD_DELTA 10//constante de proportionalité
#define KI_DELTA 10//composante d'intégration
#define KD_DELTA 10//composante de dérivation
//PID angulaire
#define KD_ALPHA 10//constante de proportionalité
#define KI_ALPHA 10//composante d'intégration
#define KD_ALPHA 10//composante de dérivation

//contraintes mecaniques
//#define ENTRAXE 10//mm
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
#define PRECISION_ALPHA 10 //precision souhaitée pour alpha (en milli radian)

//calibrage
#define TICK_PAR_MM 22 // nombre de ticks par mm (tick/mm)
#define TICK_PAR_TOUR 17145 //nombre de ticks par tour complet du robot sur lui-même (tick/tour)

//parametre pour les chemins
#define MAX_POSITIONS 20 //taille max de la liste de coordonnées definisant un chemin
#define CONSTANTE_DELTA 10 //constante pour delta quand le robot suit un chemin

#endif
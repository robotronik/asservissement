#ifndef GROS_ROBOT_H
#define GROS_ROBOT_H

/*/!\ si le robot commence à effectuer le mouvement et revient mystérieusement
en arrière c'est peut-être que vous avez un peu exagéré sur les valeurs des
coefficients des PID et qu'il y a overflow*/

//position initiale du robot
#define X_INIT 0
#define Y_INIT 0
#define THETA_INIT 0

//PID lineique
#define KP_DELTA 80 //75//constante de proportionalité
#define KI_DELTA 0 //composante d'intégration
#define KD_DELTA 5500///25000/composante de dérivation
//PID angulaire
#define KP_ALPHA 10//(200) //constante de proportionalité
#define KI_ALPHA 0    //composante d'intégration
#define KD_ALPHA 200//(85000)  //320  //composante de dérivation

//contraintes mecaniques
#define MAX_VITESSE 400000 //vitesse maximale du robot
#define MIN_VITESSE 11000 //vitesse minimale du robot pour qu'il commence à avancer
#define MAX_ACCELERATION 20 //acceleration maximale
#define MAX_DECELERATION 45 //deceleration maximale (valeur absolue)
#define VIT_MAX_ARRET 85000 //vitesse maximale à laquelle on peut s'arreter sans tomber
#define PWM_MAX 0.8  //PWM maximal, à cette valeur le robot est à sa vitesse maximale admissible
//"vitesse" correspond à la commande pour chaque moteur avant convertion en PWM
//"acceleration" et "deceleration" : différence entre deux valeurs de "vitesses" consécutives

//correction mécanique
#define COEFF_CODEUR_D 1
#define COEFF_CODEUR_G 1 //1.07
#define COEFF_MOTEUR_D 1.42//1.085
#define COEFF_MOTEUR_G 1 //1 //1.10

//precision
#define PRECISION_DELTA 10 //precision souhaitée pour delta (en mm)
#define PRECISION_ALPHA 30 //precision souhaitée pour alpha (en milli radian)

//calibrage
#define TICK_PAR_MM 7.1 // nombre de ticks par mm (tick/mm)
#define TICK_PAR_TOUR 4400 //nombre de ticks par tour complet du robot sur lui-même (tick/tour)

//parametre pour les chemins
#define MAX_POSITIONS 20 //taille max de la liste de coordonnées definisant un chemin
#define CONSTANTE_DELTA 1000 //constante pour delta quand le robot suit un chemin

//mode de déplacement par défaut
#define MODE_TRAJECTOIRE tendu //"tendu" ou "courbe"

//communication
#define ANTI_SPAM 100

#endif

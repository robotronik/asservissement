#ifndef TESTS_UNITAIRES_H
#define TESTS_UNITAIRES_H

#include "trajectoire.h"

//fonctions de débug et de calibrage du robot
void test_moteur_D(long int vitesse);
void test_moteur_G(long int vitesse);
void test_codeur_D();
void test_codeur_G();
void test_vitesse(long int vitesse);
void test_ecretage();
void test_distance(long int distance, long int vitesse);
void test_angle(long int angle, long int vitesse);
void test_asser_alpha_delta(int alpha, int delta);
void test_asser_theta(int theta);
void test_asser_xy_relatif_courbe(int x, int y);
void test_asser_xy_absolu_courbe(int x, int y);
void test_asser_xy_relatif_tendu(int x, int y);
void test_asser_xy_absolu_tendu(int x, int y);
void test_asser_chemin(s_liste chemin);
void test_sens_codeur_D();
void test_sens_codeur_G();

#endif
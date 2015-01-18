#ifndef TESTS_UNITAIRES_H
#define TESTS_UNITAIRES_H

#include "trajectoire.h"

void test_vitesse(long int vitesse);
void test_ecretage();
void test_distance(long int distance, long int vitesse);
void test_angle(long int angle, long int vitesse);
void test_asser_alpha_delta(int alpha, int delta);
void test_asser_theta(int theta);
void test_asser_xy_relatif(int x, int y);
void test_asser_xy_absolu(int x, int y);
void test_asser_chemin(s_liste chemin);

#endif
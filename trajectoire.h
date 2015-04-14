#ifndef TRAJECTOIRE_H
#define TRAJECTOIRE_H

#include "reglages.h"

typedef enum
{
	alpha_delta,
	xy_absolu,
	xy_relatif,
	chemin,
	theta,
	stop,
	emergency_stop,
	null

} e_type_trajectoire;

typedef struct
{
	int x;
	int y;
}s_coordonnee;

typedef struct
{
	s_coordonnee point[MAX_POSITIONS];
	int taille;
}s_liste;

typedef enum
{
	tendu,
	courbe
} e_mode_deplacement;

typedef struct
{
	e_type_trajectoire type;
	e_mode_deplacement mode;
	int alpha;
	int delta;
	int theta;
	int x_absolu;
	int y_absolu;
	int x_relatif;
	int y_relatif;
	s_liste chemin;

}s_trajectoire;

typedef struct
{
	long int alpha;
	long int delta;
} s_consigne;


void init_trajectoire();
void start();

//appelé par la stratégie
void set_trajectoire_alpha_delta(int alpha, int delta);
void set_trajectoire_xy_relatif(int x, int y);
void set_trajectoire_xy_absolu(int x, int y);
void set_trajectoire_theta(int new_theta);
void set_trajectoire_chemin(s_liste liste_positions);
void set_trajectoire_mode(e_mode_deplacement mode);

//utile uniquement pour du debug
int get_delta_voulu();
int get_alpha_voulu();

#endif
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
	int x;
	int y;
	s_liste chemin;

}s_trajectoire;


void init_trajectoire();
void start_asser();

//appelé par la stratégie
void build_trajectoire_alpha_delta(s_trajectoire * trajectoire, int alpha, int delta);
void build_trajectoire_xy_relatif(s_trajectoire * trajectoire, int x, int y);
void build_trajectoire_xy_absolu(s_trajectoire * trajectoire, int x, int y);
void build_trajectoire_theta(s_trajectoire * trajectoire, int new_theta);
void build_trajectoire_chemin(s_trajectoire * trajectoire, s_liste liste_positions);
void build_trajectoire_stop(s_trajectoire * trajectoire);
void build_trajectoire_emergency_stop(s_trajectoire * trajectoire); //stop définitif et violent (fin de match)
//TODO : renomer
void set_trajectoire_mode2(s_trajectoire * trajectoire, e_mode_deplacement mode);

//appelé dans l'asser pour send_position_atteinte
int trajectoire_type_is_null();

s_trajectoire * get_trajectoire();

//TODO : remove
void set_trajectoire_alpha_delta(int alpha, int delta);
void set_trajectoire_xy_relatif(int x, int y);
void set_trajectoire_xy_absolu(int x, int y);
void set_trajectoire_theta(int new_theta);
void set_trajectoire_chemin(s_liste liste_positions);
void set_trajectoire_stop();
void set_trajectoire_emergency_stop();
void set_trajectoire_mode(e_mode_deplacement mode);


#endif

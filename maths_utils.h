#ifndef MATH_PRECALC_H
#define MATH_PRECALC_H

typedef struct {
	//coordonées carthésiennes
	float x;
	float y;
	//"coordonnées polaires"
	long int delta;
	long int alpha;
	//orientation
	int theta;
} s_position;

float cos_precalc(int angle);
float sin_precalc(int angle);

#endif
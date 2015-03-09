#include "asser.h"
#include "trajectoire.h"
#include "odometrie.h"
#include "hardware.h"
#include "tests_unitaires.h"
#include "debug/affichage.h"
#include <stdio.h>
#include <pthread.h>

void * main_loop()
{
	if (init_sdl_screen() < 0)
		return NULL;
	start();
	return quit_sdl_screen();
}

int main()
{

	
	/*init*/
	init_odometrie();
	init_trajectoire();
	init_hardware();
	init_asser();

	/*chemin pour le test*/
	s_liste chemin;
	chemin.taille=7;
	chemin.point[0].x=1500;
	chemin.point[0].y=1000;
	chemin.point[1].x=2650;
	chemin.point[1].y=300;
	chemin.point[2].x=2350;
	chemin.point[2].y=1200;
	chemin.point[3].x=2700;
	chemin.point[3].y=1600;
	chemin.point[4].x=2000;
	chemin.point[4].y=800;
	chemin.point[5].x=1000;
	chemin.point[5].y=1000;
	chemin.point[6].x=250;
	chemin.point[6].y=1000;
	//chemin.point[7].x=140; //position initiale du robot
	//chemin.point[7].y=140; //position initiale du robot

	/*tests pour réglage des parametres*/

		/*tests en boucle ouverte*/
	    //test_vitesse(MIN_VITESSE);
	    //test_ecretage();
	    //test_distance(500,200000);
	    //test_angle(3142,200000);

	    /*test d'asservissement*/
	    //test_asser_alpha_delta(0,400);
	    //test_asser_alpha_delta(3140,0);
	    //test_asser_theta(3142);
	    //test_asser_xy_relatif(0,400);
	    //test_asser_xy_absolu(140,400+140);
	    //test_asser_chemin(chemin);

    /*démarage de l'asservissement*/
	//start();

	/*évite un reset automatique du microcontroleur*/
    //while (1) {;}
    pthread_t thread_asser;
    int ret;

    ret = pthread_create (&thread_asser, NULL, main_loop, NULL);
    if (ret != 0)
        fprintf(stderr, "erreur %d\n", ret);

    ret = pthread_join (thread_asser, NULL);
    if (ret != 0)
        fprintf(stderr, "erreur %d\n", ret);
	
	//return quit_sdl_screen();
}
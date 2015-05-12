#include <stdio.h>
#include <stdbool.h>

#include "hardware.h"
#if PIC_BUILD
#	if   GROS
#		include "reglages_gros.h"
#	elif PETIT
#		include "reglages_petit.h"
#	endif
#else
#	include "reglages_PC.h"
#	include <pthread.h>
#endif

#include "../common_code/communication/s2a_reception.h"

#include "asser.h"
#include "trajectoire.h"
#include "odometrie.h"
#include "tests_unitaires.h"
#if USE_SDL
#   include "simulation/affichage.h" // à virer
#endif
#include "match.h"

#include "../common_code/common.h"

////////////////////////////////////////////////////////////////////////////////

void * main_loop()
{
#if USE_SDL
	if (init_sdl_screen() < 0)
		return NULL;
	start();
    return (void*) (long) quit_sdl_screen();
#else
	start();
	return 0;
#endif
}

////////////////////////////////////////////////////////////////////////////////


int main()
{


	/*init*/
	init_odometrie();
	init_trajectoire();
	init_hardware();
	init_asser();

	/*chemin pour le test*/
	//s_liste chemin;
	//chemin.taille=7;
	//chemin.point[0].x=1500;
	//chemin.point[0].y=1000;
	//chemin.point[1].x=2650;
	//chemin.point[1].y=300;
	//chemin.point[2].x=2350;
	//chemin.point[2].y=1200;
	//chemin.point[3].x=2700;
	//chemin.point[3].y=1600;
	//chemin.point[4].x=2000;
	//chemin.point[4].y=800;
	//chemin.point[5].x=1000;
	//chemin.point[5].y=1000;
	//chemin.point[6].x=250;
	//chemin.point[6].y=1000;
	////chemin.point[7].x=140; //position initiale du robot
	////chemin.point[7].y=140; //position initiale du robot
        //test_moteur_D(400000);
        //test_moteur_G(400000);
        //test_codeur_D();
        //test_codeur_G();
	/*tests pour réglage des parametres*/

		/*tests en boucle ouverte*/
	    //test_vitesse(MIN_VITESSE);
            //test_vitesse(400000);
	    //test_ecretage();
	    //test_distance(500,200000);
	    //test_angle(3142,400000);

	    /*test d'asservissement*/
            //test_sens_codeur_D();
	    //test_asser_alpha_delta(0,13800);
	    //test_asser_alpha_delta(3142,0);
	    //test_asser_theta(3142);
	    //test_asser_xy_relatif_courbe(0,400);
	    //test_asser_xy_absolu_courbe(140,400+140);
		//test_asser_xy_relatif_tendu(0,400);
	    //test_asser_xy_absolu_tendu(140,400+140);
	    //test_asser_chemin(chemin);

//	#if PIC_BUILD
//    //démarage de l'asservissement
//	//start();
//
//	//évite un reset automatique du microcontroleur
//    while (1) {;}
//
//    #else
//	pthread_t thread_asser;
//    int ret;
//
//    ret = pthread_create (&thread_asser, NULL, main_loop, NULL);
//    if (ret != 0)
//        fprintf(stderr, "erreur %d\n", ret);
//
//    //////////
//
//    while(match_get_etat() != MATCH_FIN) {
//        // On lit l'entrée standard, et on passe les caractères à la fonctions
//        // qui gère les interruption de l'uart
//        s2a_lecture_message(getc(stdin));
//    }
//
//    //////////
//
//    ret = pthread_cancel(thread_asser);
//    if (ret != 0)
//        fprintf(stderr, "erreur %d\n", ret);
//    #endif

 extern unsigned short rxBufferDebut;
 extern unsigned short rxBufferFin;
        char c;
        extern s_consigne consigne;
        while(match_get_etat() != MATCH_FIN)
        {
            //asservissement
            asser(consigne);

            //on recalcule la position actuelle du robot (via les roues codeuses)
            actualise_position();

            //on met à jour la consigne pour l'asser
            update_consigne();

            if (UART_getc(&c)) {
                s2a_lecture_message(c);
            }
        }

        while(1) {
        }
        //while(1);
    return 0;
}

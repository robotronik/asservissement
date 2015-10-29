/*//ici la couche d'abstraction du microcontroleur utilisé
//on fait ceci afin de rendre le code portable

void set_PWM_moteur_D(int PWM)
{}

void set_PWM_moteur_G(int PWM)
{}

int get_nbr_tick_D()
{}

int get_nbr_tick_G()
{}

int doit_attendre()
{
    //synchronisation de asser() à une fréquence régulière
}

//ajouter fonction d'envoi et de réception par l'uart
//une fois quelque chose reçu on envoie à "communicution.c"*/

#define _POSIX_C_SOURCE 199309L
//#include <signal.h>

#include <math.h>
#include "hardware.h"
#include <stdio.h>
#include <pthread.h>
#include <time.h>



#include "../hardware/debug.h"



#if USE_SDL
#   include "../hardware/PC/hardware.h"
#   include "odometrie.h"
#endif

#define RX_BUFFER_SIZE 40
#define SYNCHRO_TIME 0.016

long int PWM_D;
long int PWM_G;
int moteurs_arret=0;
long preced_clock=0;

/*void alarm_signal_handler(int signum)
{
    //réassignation du handler pour le signal d'alarme
    signal(SIGALRM,alarm_signal_handler);
}

void init_timer()
{
    //creation du timer
    timer_t timer_periode_asser;
    timer_create(CLOCK_REALTIME, NULL, &timer_periode_asser);
    //assignation du handler pour le signal d'alarme
    signal(SIGALRM,alarm_signal_handler);
    //période du timer
    struct timespec periode;
    periode.tv_sec=0;
    periode.tv_nsec=SYNCHRO_TIME*1000000000;
    //prochaine expiration du timer (à chaque fois remis à la valeur "période")
    struct timespec next_expiration=periode;
    //construction de la structure de data pour timer_settime()
    struct itimerspec timer_data;
    timer_data.it_interval=periode;
    timer_data.it_value=next_expiration;
    //armement du timer qui déclanchera le signal d'alarme (SIGALRM)
    timer_settime(timer_periode_asser,0,&timer_data,NULL);
}*/

void init_hardware()
{
    init_UART();
    //init_timer();
}

void set_PWM_moteur_D(int PWM)
{
    if (!moteurs_arret)
    {
        PWM_D+=PWM/10;
    }
}

void set_PWM_moteur_G(int PWM)
{
    if (!moteurs_arret)
    {
        PWM_G+=PWM/10;
    }
}

long int get_nbr_tick_D()
{
    return PWM_D*5;
}

long int get_nbr_tick_G()
{
    return PWM_G*5;
}

void attente_synchro()
{
    //structute du temps d'attente
    struct timespec sleep_time;
    //temps d'attente (secondes)
    sleep_time.tv_sec=0;
    //horloge actuelle
    long actual_clock= (long) clock();
    //calcul du temps d'attente (nanosecondes)
    sleep_time.tv_nsec=SYNCHRO_TIME*1000000000-(actual_clock-preced_clock)*1000000000/CLOCKS_PER_SEC;
    //attente
    nanosleep(&sleep_time,NULL);
    //sauvegarde de l'ancienne horloge
    preced_clock=actual_clock;
}

void motors_stop()
{
    moteurs_arret=1;
}


void allumer_del()
{

}

void eteindre_del()
{

}

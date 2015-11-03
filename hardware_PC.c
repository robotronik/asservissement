#define _POSIX_C_SOURCE 199309L
#include <math.h>
#include <time.h>

#include "hardware.h"
#include "debug.h"


void UART_send_message(void* a) {

}

#define RX_BUFFER_SIZE 40
#define SYNCHRO_TIME 0.016

long int PWM_D = 0;
long int PWM_G = 0;

int moteurs_arret = 0;
long preced_clock = 0;

void init_hardware()
{
    init_UART();
}

void set_PWM_moteur_D(int PWM)
{
    if (!moteurs_arret)
        PWM_D+=PWM/2;
}

void set_PWM_moteur_G(int PWM)
{
    if (!moteurs_arret)
        PWM_G+=PWM/2;
}

long int get_nbr_tick_D()
{
    return PWM_D;
}

long int get_nbr_tick_G()
{
    return PWM_G;
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
{ }

void eteindre_del()
{ }

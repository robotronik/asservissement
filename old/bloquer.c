#include "bloquer.h"
#include "main.h"
char nbTimer;//Nombre de fois que l'interruption est déclanché
void start_bloquer()
{
    T2CONbits.TON = 1; //On start le timer
    if(nbTimer > NB_TIMER)//On est bloqué
    {
        allumer_del();
        //sendDerapage();
    }
}

void reset_bloquer()
{
    nbTimer = 0;
    TMR2 = 0;

    T2CONbits.TON = 0; //On stop le timer
    eteindre_del();
}

void __attribute__((__interrupt__, no_auto_psv)) _T2Interrupt(void)
{
    nbTimer++;
    IFS0bits.T2IF = 0;
}

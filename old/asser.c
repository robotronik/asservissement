/*-----------------------------------------------------*
 * Calcul des pid et asservissement                    *
 *-----------------------------------------------------*/
#include "main.h"
#include "motors.h"
#include "message.h"
#include "bloquer.h"
#include <math.h>

#ifdef SIMU
#include "prog2.h"
#endif


extern int isPaused;
extern unsigned char commandeSaved;
extern unsigned short consigneXSaved, consigneYSaved;


volatile boolean doitAttendre = TRUE; // Echantilloneur


//////////////////////////////

long int AD_consAlpha =(long) 0;//10*(TICK_TOUR);//0;
long int AD_consDelta =0;// 1000*MM_TO_TICKS; //1000* MM_TO_TICKS;//3000*MM_TO_TICKS; //1000*MM_TO_TICKS;
long int AD_ecartDelta = 0;
long int AD_ecartAlpha = 0;


volatile AD_K kP_delta;
volatile AD_K kD_delta;

volatile AD_K kP_alpha;
volatile AD_K kD_alpha;

long int AD_distG = 0; //en tick
long int AD_distD = 0;
long int AD_precG = 0; //en tick
long int AD_precD = 0;


long int AD_dG = 0; //en tick
long int AD_dD = 0;

long int AD_presDelta ;//= 20 * MM_TO_TICKS; //20*MM_TO_TICKS;//MM_TO_TICKS;//1.*10950./1000.;// 5mm // en mm
long int AD_presAlpha ;//= 3.0 * TICK_TOUR / 360.0; //200;//1.0*(TICK_TOUR/360.0); //


long int AD_alpha = 0;
long int AD_delta = 0;
long int AD_precDelta = 0;
long int AD_commandeDelta = 0;
long int AD_commandePrecDelta = 0;
long int AD_commandePrecAlpha = 0;
long int AD_commandeAlpha = 0;
long int AD_alphaTotal = 0; //en ticks
volatile double alphaRad = 0;


volatile char reset = 0;

char done = 1;

volatile long int acceleration_max;
extern long int AD_minSpeed;

extern volatile char freeWheel;

/* inulite à supprimer*/
int abs(int a)
{
    if (a < 0)
        return -a;

    return a;
}

void asser()
{

    long int erreurs_delta[4] = {0, 0, 0, 0};
    long int erreurs_alpha[4] = {0, 0, 0, 0};
    int i;
    int cmdD, cmdG;
    static int cptDerap = 0;
    static int unsigned cptDone = 0;
    static unsigned int antispam = 1001;
    const unsigned int reSendDone = 1600; //On renvoie un sendDone toute les 1s
    ///////////////////////////
    motors_reset_qei();

    setGros();
    
    /////////////////////////////////

    while (1)
    {

        while (doitAttendre); // Attend l'interruption du timer pour une fréquence régulière
        doitAttendre = TRUE;

        //_RB5 = !_RB5;

        AD_commandePrecDelta = AD_commandeDelta;
        AD_commandePrecAlpha = AD_commandeAlpha;
        if (reset)
        {

            reset = 0;
            for (i = 0; i < 4; i++)
            {
                erreurs_delta[i] = 0;
                erreurs_alpha[i] = 0;
            }
            AD_delta = 0;
            AD_precDelta = 0;
            AD_alpha = 0;
            AD_commandePrecDelta = 0;
            AD_commandePrecAlpha = 0;

            eteindre_del();
            done = 0;


        }
        /*****Detection du dérapage*****/
        ////////////////////////////////
         //On regarde si nous avons des commandes mais que nousn'arrivons pas à bouger
        /*if ((AD_abs(AD_commandeDelta - AD_commandeAlpha) > AD_MIN_SPEED || AD_abs(AD_commandeDelta + AD_commandeAlpha) > AD_MIN_SPEED)
                && (AD_abs(AD_dD - AD_dG) < 5 && AD_abs(AD_dD + AD_dG) < 5))
        {
            start_bloquer();
        }
        else
        {
            reset_bloquer();
        }*/
        // commande precedente des moteurs
        
        cmdG = AD_commandeDelta - AD_commandeAlpha;
        cmdD = AD_commandeDelta + AD_commandeAlpha;
        if (antispam <= 1001)
            antispam++;
        if (AD_abs(cmdG) > (AD_minSpeed * SEUIL_DERAP) && AD_abs(AD_dG) < 2 && antispam > 1000)
        {
            if (AD_abs(cmdD) > (AD_minSpeed * SEUIL_DERAP) && AD_abs(AD_dD) < 1)
            {
                cptDerap++;
                if (cptDerap == 10)
                {
                    sendDerapage();
                    cptDerap = 0;
                    antispam=0;
                }
            }
            else
                cptDerap = 0;

        }
        else
            cptDerap = 0; //*/

        ////////////////////////////////
        AD_precG = AD_distG;
        AD_precD = AD_distD;
        motors_get_qei(&AD_distG, &AD_distD);

        //Calcul des variations
        AD_dG = AD_distG - AD_precG;
        AD_dD = AD_distD - AD_precD;



        AD_alpha += (AD_dD - AD_dG);

        AD_delta += (AD_dD + AD_dG);
        //                     dAlpha       / distEntraxeTicks     dDelta
        majXYAngle((double) (((COEFF_DIFF_ROUE*AD_dD - AD_dG)) / (TICK_TOUR / DEUX_PI)), (double) (AD_dD + COEFF_DIFF_ROUE*AD_dG));
        AD_precDelta = AD_delta;
        //alpha
        AD_ecartAlpha = AD_consAlpha - AD_alpha;
        maj_erreurs(erreurs_alpha, AD_ecartAlpha);
        if (AD_consAlpha == 0) //Delta pur
            AD_commandeAlpha = AD_ecartAlpha * kP_delta.alpha + deriv_erreurs(erreurs_alpha) * kD_delta.alpha;
        else
            AD_commandeAlpha = AD_ecartAlpha * kP_alpha.alpha + deriv_erreurs(erreurs_alpha) * kD_alpha.alpha;

        //delta
        AD_ecartDelta = AD_consDelta - AD_delta;

        maj_erreurs(erreurs_delta, AD_ecartDelta);

        if (AD_consAlpha == 0) //Deltapur
            AD_commandeDelta = AD_ecartDelta * kP_delta.delta + deriv_erreurs(erreurs_delta) * kD_delta.delta;
        else // alpha pur
            AD_commandeDelta = AD_ecartDelta * kP_alpha.delta + deriv_erreurs(erreurs_delta) * kD_alpha.delta;
        
                //On regarde si nous avons des commandes mais que nousn'arrivons pas à bouger 
                /*if(((AD_commandeDelta  - AD_commandeAlpha) != 0 || (AD_commandeDelta  + AD_commandeAlpha) != 0) && (AD_abs(AD_dD) < 2 && AD_abs(AD_dG) < 2) )
                {
                    start_bloquer();
                }
                else
                {
                    reset_bloquer();
                }*/

        //Rampe de vitesse
        if (AD_consAlpha == 0) //Delta pur
            AD_commandeDelta = rampe(AD_commandeDelta, AD_commandePrecDelta);
        else
            AD_commandeAlpha = rampe(AD_commandeAlpha, AD_commandePrecAlpha);



        //Mise à l'échelle des PWN


        if ((AD_abs(AD_ecartAlpha) < AD_presAlpha) && (AD_abs(AD_ecartDelta) < AD_presDelta)) //On test si on est au bon endroit
        {
            if (((AD_max((AD_commandeDelta - AD_commandeAlpha), (AD_commandeDelta + AD_commandeAlpha)) < VIT_MAX) && (AD_consAlpha == 0)) || (AD_consDelta == 0)) // Evite que le robot tombe en arrière
            {


                AD_commandeAlpha = 0;
                AD_commandeDelta = 0;
                if (!done || (cptDone > reSendDone) )
                    sendDone();
                done = 1;

                cptDone++;
                if(cptDone > reSendDone)
                {
                    cptDone = 0;
                }
            }
            else
                cptDone = 0;
        }
        else
            cptDone = 0;


       
        if(!freeWheel)
        AD_motors_set_speed((short) echelle(AD_commandeDelta - AD_commandeAlpha), (short) echelle(AD_commandeDelta + AD_commandeAlpha));
        else
            AD_motors_set_speed(0,0);
        

        //DEBUG //a enlever pour la coupe
        if (!doitAttendre) {while(1){allumer_del(); pause_ms(500); eteindre_del(); pause_ms(500);}} // Timer trop rapide : allume LED debug
    }
}

void __attribute__((__interrupt__, no_auto_psv)) _T1Interrupt(void)
{
    doitAttendre = 0;
    IFS0bits.T1IF = 0;
    TMR1 = 0;
}
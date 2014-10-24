/*-----------------------------------------------------*
 * Protocole de ommunication avec l'uart               *
 *-----------------------------------------------------*/

#include "main.h"
#include "message.h"
#include "asser.h"
#include "motors.h"
#include "uart.h"

extern volatile int isPSoCReady;

#define X_START			LONG_ARRIERE
#define Y_START			250	// [mm]


extern volatile char reset;

//Asser 
extern long int AD_consAlpha; //Angle en rad
extern long int AD_consDelta; //Delta : Distance en mm * Distance parcourue par un tour de roue -> en tick 

extern long int AD_alphaTotal;
extern volatile double alphaRad;

extern volatile AD_K kP_delta;
extern volatile AD_K kD_delta;
extern volatile AD_K kP_alpha;
extern volatile AD_K kD_alpha;

extern volatile double dXdouble;
extern volatile double dYdouble;

volatile char freeWheel = 0;


void message_processing(unsigned char inMessage)
{
    static char nb_recu = 0;
    static int boolConfig = 0;
    static TrameAsser t;
    static TrameConfig tc;
    short valeur;
    Trame32 t32;
    TrameAsser t16;


    if (nb_recu == 0)
    {
        t.trShort.Xo = inMessage;
        if (t.trGenShort.codet == 0b0100|| t.trGenShort.codet == 0b0101 ||
                t.trGenShort.codet == CODE_ALPHA || t.trGenShort.codet == CODE_STOP ||
                t.trGenShort.codet == CODE_CONFIG || t.trGenShort.codet == CODE_GET_XY ||
                t.trGenShort.codet == CODE_GET_ALPHA || t.trGenShort.codet == CODE_SET_XY ||
                t.trGenShort.codet == CODE_ALPHA_ABS ||
                t.trGenShort.codet == CODE_PRES_GROS ||
                t.trGenShort.codet == CODE_PRES_PRECIS ||
                t.trGenShort.codet == CODE_SET_ALPHA || t.trGenShort.codet == CODE_FREE_WHEEL
                )
            nb_recu++;
    }
    else if (nb_recu == 1)
    {
        t.trShort.oX = inMessage;

        switch (t.trGenShort.codet)
        {

            /*Cas du delta*/
            /*C'set moche mais visiblement case 0x4 || 0x5: marche pas*/
            case 0x4 :
            if (t.trDelta.signe == 1)
                valeur = -t.trDelta.delta;
            else
                valeur = t.trDelta.delta;
            //AD_consDelta = valeur*10950./1000.;
            AD_consDelta = valeur*MM_TO_TICKS;
            AD_consAlpha = 0;
            nb_recu = 0;
            reset = 1;
            freeWheel = 0;
            break;

            case 0x5 :
            if (t.trDelta.signe == 1)
                valeur = -t.trDelta.delta;
            else
                valeur = t.trDelta.delta;
            //AD_consDelta = valeur*10950./1000.;
            AD_consDelta = valeur*MM_TO_TICKS;
            AD_consAlpha = 0;
            nb_recu = 0;
            reset = 1;
            freeWheel = 0;
            break;

            /*Autre cas*/

        case CODE_ALPHA: //0b101 : //alpha
            if (t.trAlpha.signe == 1)
                valeur = -t.trAlpha.alpha;
            else
                valeur = t.trAlpha.alpha;

            AD_consDelta = 0;
            //AD_consAlpha = valeur*3.14/180.0;
            AD_consAlpha = valeur * TICK_TOUR / 360;
            nb_recu = 0;
            reset = 1;
            freeWheel = 0;
            break;

        case CODE_ALPHA_ABS:
            if (t.trAlpha.signe == 1)
                valeur = -t.trAlpha.alpha;
            else
                valeur = t.trAlpha.alpha;
            long int actuel = AD_alphaTotal; //alpha actuel
            actuel %= (long int)TICK_TOUR;
            long int demande = (long int)valeur;
            demande *= TICK_TOUR;
            demande /= 360;
            demande %= (long int) TICK_TOUR;
            long int diff = demande - actuel;
            if (diff > TICK_TOUR/2)
            {
                diff = diff - TICK_TOUR;

            }
            else if (diff < -(TICK_TOUR/2))
            {
                diff = TICK_TOUR + diff;
            }
            AD_consDelta = 0;
            AD_consAlpha = diff;
            nb_recu = 0;
            reset = 1;
            freeWheel = 0;
            break;
            
        case CODE_SET_ALPHA:
            nb_recu = 0;
            if (t.trAlpha.signe == 1)
                valeur = -t.trAlpha.alpha;
            else
                valeur = t.trAlpha.alpha;
            alphaRad = (double)valeur*PI/180.0;//conversion en radian
            AD_alphaTotal = alphaRad*(TICK_TOUR/DEUX_PI);//conversion en ticks
            break;

        case CODE_STOP: //0b0000 :
            AD_consDelta = 0;
            AD_consAlpha = 0;
            nb_recu = 0;
            reset = 1;
            freeWheel = 0;
            break;

        case CODE_CONFIG: //0b111
            boolConfig = 1;
            tc.tr4Char.Xooo = t.trShort.Xo;
            tc.tr4Char.oXoo = t.trShort.oX;
            nb_recu++;
            break;

        case CODE_GET_XY:
            nb_recu = 0;

            t32.tpos.x = (int)(dXdouble*TICKS_TO_MM);
            t32.tpos.y = (int)(dYdouble*TICKS_TO_MM);
            if ((int)t32.tpos.y < 0)
                t32.tpos.y = 0;
            if ( (int)t32.tpos.x < 0)
                t32.tpos.x = 0;

            t32.tpos.codet = CODE_GET_XY;
            send_long(t32.l);
            break;

        case CODE_GET_ALPHA:
            nb_recu = 0;
            if ((AD_alphaTotal ) < 0)
            {
                unsigned long alphaCalc = -(AD_alphaTotal);
                alphaCalc %= (long int) TICK_TOUR;
                alphaCalc *= 360;
                alphaCalc /= (unsigned long int) TICK_TOUR;
                t16.trAlpha.alpha = alphaCalc;
                t16.trAlpha.signe = 1;
            }
            else
            {
                unsigned long alphaCalc = (AD_alphaTotal);
                alphaCalc %= (long int) TICK_TOUR;
                alphaCalc *= 360;
                alphaCalc /= (unsigned long int) TICK_TOUR;
                t16.trAlpha.alpha = alphaCalc;
                t16.trAlpha.signe = 0;
            }
            t16.trAlpha.codet = CODE_GET_ALPHA;
            send_short(t16.us);
            break;

        case CODE_SET_XY:
            nb_recu++;
            tc.tr4Char.Xooo = t.trShort.Xo;
            tc.tr4Char.oXoo = t.trShort.oX;
            break;

        case CODE_PRES_GROS:
            nb_recu = 0;
            setGros();
            break;

        case CODE_PRES_PRECIS:
            nb_recu = 0;
            setPrecis();
            break;

        case CODE_FREE_WHEEL :
            freeWheel = 1;
            nb_recu = 0;
            break;

            

        }


    }
    else if (nb_recu == 2)
    {
        tc.tr4Char.ooXo = inMessage;
        nb_recu++;
    }
    else
    {
        tc.tr4Char.oooX = inMessage;
        if (tc.tpos.codet == CODE_SET_XY)//Le psoc veut définir la position
        {

            int px = (int)tc.tpos.x;
            int py = (int)tc.tpos.y;
            // on l'un des param est -1 on ne le met pas a jour
            if (px < 0xFFF)//sur 12 bits
                dXdouble = (double) (px * MM_TO_TICKS);
            if (py < 0xFFF)
                dYdouble = (double) (py * MM_TO_TICKS);

            if (dYdouble < 0)
                dYdouble = 0;
            if (dXdouble < 0)
                dXdouble = 0;


        }
        else
        {
            switch (tc.conf.pid)
            {
            case CODE_P: //0b00
                switch (tc.conf.ad1)
                {
                case CODE_CONFIG_DELTA: //0b0
                    switch (tc.conf.ad2)
                    {
                    case CODE_CONFIG_DELTA: // 0b0
                        kP_delta.delta = tc.conf.ent; //(double)(tc.conf.ent + tc.conf.dec/100.);
                        break;
                    case CODE_CONFIG_ALPHA: // 0b1
                        kP_delta.alpha = tc.conf.ent; //(double)(tc.conf.ent + tc.conf.dec/100.);
                        break;
                    }
                    break;
                case CODE_CONFIG_ALPHA: //0b1
                    switch (tc.conf.ad2)
                    {
                    case CODE_CONFIG_DELTA: // 0b0
                        kP_alpha.delta = tc.conf.ent; //(double)(tc.conf.ent + tc.conf.dec/100.);
                        break;
                    case CODE_CONFIG_ALPHA: // 0b1
                        kP_alpha.alpha = tc.conf.ent; //(double)(tc.conf.ent + tc.conf.dec/100.);
                        break;
                    }
                    break;
                }
                break;
            case CODE_D: //0b10
                switch (tc.conf.ad1)
                {
                case CODE_CONFIG_DELTA: //0b0
                    switch (tc.conf.ad2)
                    {
                    case CODE_CONFIG_DELTA: // 0b0
                        kD_delta.delta = tc.conf.ent; //(double)(tc.conf.ent + tc.conf.dec/100.);
                        break;
                    case CODE_CONFIG_ALPHA: // 0b1
                        kD_delta.alpha = tc.conf.ent; //(double)(tc.conf.ent + tc.conf.dec/100.);
                        break;
                    }
                    break;
                case CODE_CONFIG_ALPHA: //0b1
                    switch (tc.conf.ad2)
                    {
                    case CODE_CONFIG_DELTA: // 0b0
                        kD_alpha.delta = tc.conf.ent; //(double)(tc.conf.ent + tc.conf.dec/100.);
                        break;
                    case CODE_CONFIG_ALPHA: // 0b1
                        kD_alpha.alpha = tc.conf.ent; //(double)(tc.conf.ent + tc.conf.dec/100.);
                        break;
                    }
                    break;
                }
                break;

            }
        }
        nb_recu = 0;
    }

}

void fin(void)
{
    motors_stop();
    while (1)
    {
        allumer_del();
        pause_ms(500);
        eteindre_del();
        pause_ms(500);
    }
}

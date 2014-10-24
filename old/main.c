/*-----------------------------------------------------*
 * Main                                                *
 *-----------------------------------------------------*/

#include "main.h"
#include "message.h"
#include "motors.h"
#include "callage.h"
#include "init.h"

//#include "bitsConfiguration.h"
volatile int isPSoCReady = FALSE;


extern long int AD_minSpeed;


int main() {
    long int tickG = 0, tickD = 0;
    long int vG=7900, vD=7900;
    init();


    while (tickG < 10 && tickD < 10)
    {
        vG += 20;
        vD += 20;
        tickG = 0;
        tickD = 0;
        motors_reset_qei();
        motors_set_speed(echelle(vG), echelle(vD));
        pause_ms(70);
        motors_get_qei(&tickG, &tickD);
    }
    motors_set_speed(0, 0);
    AD_minSpeed = (long int)((float)vG*1.5);
    asser();
    
    return 0;
}



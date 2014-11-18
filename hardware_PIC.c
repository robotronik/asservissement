/*//ici la couche d'abstraction du microcontroleur utilisé
//on fait ceci afin de rendre le code portable*/

#include "hardware.h"
#include <p33FJ128MC802.h>


int doitAttendre;

void pause_ms(unsigned short n)
{
    unsigned short i,j;
    for(j=0; j<n; j++)
    {
        for(i=0; i<4000; i++)
            Nop();
    }
}

void pause_s(unsigned short n)
{
    unsigned short i;
    for(i=0; i<n; i++)
        pause_ms(1000);
}

/*----------------------------------------------------------------*
* Init du registre et des variables                                 *
*-----------------------------------------------------------------*/

_FOSCSEL(FNOSC_FRCPLL & IESO_ON)
_FOSC(FCKSM_CSECMD & IOL1WAY_ON & OSCIOFNC_ON & POSCMD_NONE)
_FWDT(FWDTEN_OFF & WINDIS_OFF & WDTPRE_PR128 & WDTPOST_PS32768)

void init_osc()
{
	// Init de la PLL : 7,37 -> 79,23 MHz !!!
	CLKDIVbits.PLLPRE = 0;		// (FRC) /2
	PLLFBD = 41;				// (FRC/2) *43
	CLKDIVbits.PLLPOST = 0;		// (FRC/2*43) /2
	
	__builtin_write_OSCCONH((OSCCONH | 1 )& 0xF9);	// Choix de l'horloge FRC avec PLL
	__builtin_write_OSCCONL(OSCCONL | 1);			// Changement d'horloge
	//while (!OSCCONbits.LOCK);						// Attend le bloquage de la PLL (debug)
	


	while (OSCCONbits.OSWEN); 						// Attend le changement
}

void io_init()
{
	// "All port pins are defined as inputs after a Reset"

	// Reglage du PORTA
	//_TRISA0 = 1;	// MOTOR_LEFT_CURRENT
	//_TRISA1 = 1;
	//_TRISA2 = 1;	// MOTOR_SF

	// Reglage du PORTB
	//U1MODEbits.UARTEN = 0b0; // UART déconnecté
	//_TRISB2 = 0;
	_TRISB3 = 1;

	_TRISB4 = 0;	// Motor_enable
	_RB4 = 1;
	pause_ms(1);
	_TRISB5 = 0;	// DEL en mode output
	_RB5 = 1;	 	// On eteint la DEL par defaut
	//_TRISB6 = 1;	// Coder right B
	//_TRISB7 = 1;	// Coder right A
	//_TRISB10 = 1;	// Coder left B
	//_TRISB11 = 1;	// Coder left A

	_TRISB12 = 0;	// MOTEUR IN1 right
	_RB12 = 0;		// PWM en IO général si non-utilisé (au moins 2 broches sur 4)
	_TRISB13 = 0;	// MOTEUR IN2 right
	_RB13 = 0;
	_TRISB14 = 0;	// MOTEUR IN1 left
	_RB14 = 0;
	_TRISB15 = 0;	// MOTEUR IN2 left
	_RB15 = 0;
}

/** Initialise le module PWM PWM1 **/
void PWM1_init ()
{
	//P1TCONbits.PTSIDL = 1;	// Arret en pause

    P1TPER = MAX_SPEED/2;		// Période du timer
    P1DC1 = 0;                 	// Duty-cycle PWM1H1 = 0%
    P1DC2 = 0;                 	// Duty-cycle PWM1H2 = 0%

    PWM1CON1bits.PMOD1 = 1;     // Sorties PWM1H1 et PWM1L1 indépendantes
    PWM1CON1bits.PMOD2 = 1;     // Sorties PWM1H2 et PWM1L2 indépendantes
    motors_stop();

    P1TCONbits.PTEN = 1;        // Active le Timer des PWMs
    P1TCONbits.PTOPS = 5-1;	// Reset automatique aux modif de P1TCON
	
}

void timer_init()
{

/********************************************/
/*Initialisation de la période de l'asser   */
/********************************************/
	T1CONbits.TSIDL = 1;
	T1CONbits.TGATE  = 0 ;
	T1CONbits.TCKPS = 0b11;//Prescaller 256

	TMR1 = 0; //Reset du conteur


	PR1 = 100;//250; //Réglage de la fréquence de l'horloge. (Fctimer = (40Mhz/256))

	T1CONbits.TCS = 0;

	IPC0bits.T1IP = 0x01; // Set Timer1 Interrupt Priority Level
	IFS0bits.T1IF = 0; // Clear Timer1 Interrupt Flag
	IEC0bits.T1IE = 1; //Enable interupt

	T1CONbits.TON = 1;//Début du timer

/********************************************/
/*Initialisation du timer de blocage  16bit */
/********************************************/

	T2CONbits.TON = 0; //On stop le timer
	T2CONbits.TSIDL = 1;

	T2CONbits.TGATE  = 0 ;
	T2CONbits.TCKPS = 0b11;//Prescaller 64

	T2CONbits.T32 = 0; //Timer 16bits
	T2CONbits.TCS = 0; //Horloge interne

	PR2 = 12438; //Timer de 10ms
	TMR2 = 0;

	IPC1bits.T2IP = 0x01; // Set Timer1 Interrupt Priority Level
	IFS0bits.T2IF = 0; // Clear Timer1 Interrupt Flag
	IEC0bits.T2IE = 1; //Enable interupt
}	


void QEI_init ()
{
    /* Initialise les décodeurs en quadrature QEI1 et QEI2 */

    /* Désactivation QEI avant configuration */
    QEI1CONbits.QEIM   = 0b000;
    QEI2CONbits.QEIM   = 0b000;

    /* Connection des modules QEI1 : LEFT et QEI2 : RIGHT aux pins RPx du PIC */
    RPINR14bits.QEA1R  = 11;             // QEA1 sur RP11
    RPINR14bits.QEB1R  = 10;             // QEB1 sur RP10
    RPINR16bits.QEA2R  = 6;              // QEA2 sur RP6
    RPINR16bits.QEB2R  = 7;              // QEB2 sur RP7

    /* Initialisation QEI1 (roue gauche) */
	QEI1CONbits.QEISIDL = 1;			// Lors de la mise en pause le compteur s'arrete
    QEI1CONbits.SWPAB  = 1;            	// A et B inversés
    DFLT1CONbits.CEID  = 1;             // Pas d'interruption sur erreur
	MAX1CNT = 65535;					// Valeur maximale du compteur

    /* Initialisation QEI2 (roue droite) */
    /* Sensiblement la même chose */
	QEI2CONbits.QEISIDL = 1;
    QEI2CONbits.SWPAB  = 1;
    DFLT2CONbits.CEID  = 1;
	MAX2CNT = 65535;

    /* Reset compteurs */
    POS1CNT = 0;
    POS2CNT = 0;
	
	/* Activation des interruptions sur débordement des compteurs */
	IEC3bits.QEI1IE = 1;
	IEC4bits.QEI2IE = 1;

    /* Activation QEI1 et QEI2 */
    QEI1CONbits.QEIM   = 0b111;          // Mode 4x, reset et interruption sur MAXxCNT
    QEI2CONbits.QEIM   = 0b111;
}

void UART_init()
{
	AD1PCFGLbits.PCFG5 = 1;	// Désactivation de l'entrée analogique !!!
	RPOR1bits.RP2R = 3;							// Tx1 -> RP2
	RPINR18bits.U1RXR = 3;						// RP3 -> Rx1

	//U1MODEbits.LPBACK = 1;	// Debug : Tx1 -> Rx1

	// Low speed : BRG = 79,23 MHz / 32 / Baudrate - 1
	U1MODEbits.BRGH = 1;	// High speed : BRG = 79,23 MHz / 8 / Baudrate - 1
	U1BRG = 85; 			// BAUD Rate Setting for 115200 gives 115160 bauds
	
#ifdef INT_UART_TX
	U1STAbits.UTXISEL1 = 1;	// Interrupt on empty FIFO, last byte is being sent
	U1STAbits.UTXISEL0 = 0;	//                      "
#endif //#ifdef INT_UART_TX

	IFS0bits.U1RXIF = 0; 	// On evite des interruptions à l'activation
	IEC0bits.U1RXIE = 1;	// Activation de l'interruption sur réceptions
#ifdef INT_UART_TX
	IFS0bits.U1TXIF = 0;
	IEC0bits.U1TXIE = 1;	// Activation de l'interruption sur l'envoie
#endif //#ifdef INT_UART_TX
	//IFS4bits.U1EIF = 0;
	//IEC4bits.U1EIE = 1;	// Activation de l'interruption sur erreurs

	U1MODEbits.UARTEN = 1; 	// Enable UART
	U1STAbits.UTXEN = 1; 	// Enable UART TX
}

void init(
{
	init_osc();
	//__builtin_write_OSCCONL(OSCCON & ~(0x40));	// Débloquage des RPIN et RPOR
    io_init();
    PWM1_init ();
    QEI_init ();
    UART_init();
    timer_init();
	//__builtin_write_OSCCONL(OSCCON | 0x40);		// Rebloquage des RPIN et RPOR
}

/*----------------------------------------------------------------*
 * génération des PWMs de commande des moteurs                    *
 *----------------------------------------------------------------*/
void set_PWM_moteur_D(int PWM)
{
	short speedR=(short) PWM/1000*MAX_SPEED;
   	if (speedR == 0)    // On s arrete
    {
        PWM1CON1bits.PEN2H = 0;
        PWM1CON1bits.PEN2L = 0;
    }
    else if(speedR >= MAX_SPEED){
        PWM1CON1bits.PEN2H = 0;
        PWM1CON1bits.PEN2L = 1;
        P1DC2 = MAX_SPEED;
    }
    else if(speedR <= -MAX_SPEED){
        PWM1CON1bits.PEN2H = 1;
        PWM1CON1bits.PEN2L = 0;
        P1DC2 = MAX_SPEED;
    }
	else if (speedR > 0)  // On avance
    {
        PWM1CON1bits.PEN2H = 0;
        PWM1CON1bits.PEN2L = 1;
        P1DC2 = speedR;
    }
   	else if (speedR < 0)   // On recule
    {
        PWM1CON1bits.PEN2H = 1;
        PWM1CON1bits.PEN2L = 0;
        P1DC2 = -speedR;
    }
}

void set_PWM_moteur_G(int PWM)
{
	short speedL=(short) PWM/1000*MAX_SPEED;
	if (speedL == 0)    // On s arrete
    {
        PWM1CON1bits.PEN1H = 0;	// /!\ Reset le timer
        PWM1CON1bits.PEN1L = 0;
    }
    else if(speedL >= MAX_SPEED){
        PWM1CON1bits.PEN1H = 0;
        PWM1CON1bits.PEN1L = 1;
        P1DC1 = MAX_SPEED;
    }
    else if(speedL <= -MAX_SPEED){
        PWM1CON1bits.PEN1H = 1;
        PWM1CON1bits.PEN1L = 0;
        P1DC1 = MAX_SPEED;
    }
	else if (speedL > 0)  // On avance
    {
	    PWM1CON1bits.PEN1H = 0;
	    PWM1CON1bits.PEN1L = 1;
	    P1DC1 = speedL;
    }
   	else if (speedL < 0)   // On recule
    {
        PWM1CON1bits.PEN1H = 1;
        PWM1CON1bits.PEN1L = 0;
        P1DC1 = -speedL;
    }
}

void motors_stop(void)
{
	// On met les 4 sorties en logique
	PWM1CON1bits.PEN1H = 0;
    PWM1CON1bits.PEN2H = 0;
    PWM1CON1bits.PEN1L = 0;
    PWM1CON1bits.PEN2L = 0;
}

/*----------------------------------------------------------------*
 * gestion des ticks effectués sur les roues codeuses             *
 *----------------------------------------------------------------*/
short distLHigh, distRHigh;

long get_nbr_tick_D()
{
	long pPosR;
	((T_dividedLong *) pPosR)->part.high = distRHigh;
	((T_dividedLong *) pPosR)->part.low = POS2CNT;
	return pPosR;
}

long get_nbr_tick_G()
{
	long pPosL;
	((T_dividedLong *) pPosL)->part.high = distLHigh;
	((T_dividedLong *) pPosL)->part.low = POS1CNT;
	return pPosL;
}

void reset_nbr_tick()
{
	POS1CNT = 0;
	distLHigh = 0;
	POS2CNT = 0;
	distRHigh = 0;
}

void __attribute__((interrupt, auto_psv)) _QEI1Interrupt(void)
{
	IFS3bits.QEI1IF = 0; // On s'acquitte de l'interruption
	if ((unsigned short) POS1CNT < 32768) distLHigh ++;
	else distLHigh --;
}

void __attribute__((interrupt, auto_psv)) _QEI2Interrupt(void)
{
	IFS4bits.QEI2IF = 0; // On s'acquitte de l'interruption
	if ((unsigned short) POS2CNT < 32768) distRHigh ++;
	else distRHigh --;
}

/*----------------------------------------------------------------*
 * Timer pour la synchronisation de asser()                       *
 *----------------------------------------------------------------*/
int doit_attendre()
{
	return doitAttendre;
}

void __attribute__((__interrupt__, no_auto_psv)) _T1Interrupt(void)
{
    doitAttendre = 0;
    IFS0bits.T1IF = 0;
    TMR1 = 0;
}

/*----------------------------------------------------------------*
 * fonctions de debug                                             *
 *----------------------------------------------------------------*/

void allumer_del(void)
{
	_RB5 = 0; // nouvelle carte
}

void eteindre_del(void)
{
	_RB5 = 1; // nouvelle carte
}

void pause_ms(unsigned short n)
{
	unsigned short i,j;
	for(j=0; j<n; j++)
	{
     	for(i=0; i<4000; i++)
			Nop();
	}
}

void pause_s(unsigned short n)
{
	unsigned short i;
	for(i=0; i<n; i++)
		pause_ms(1000);
}

/*---------------------------------------------------------------------------*
 * Couche basse uart                                                         *
 *---------------------------------------------------------------------------*/


/*--- variables and define ---*/

unsigned char rxBuffer[RX_BUFFER_SIZE];
volatile unsigned short indexRxBuffer = 0;
volatile unsigned short rxBufferLength = 0;

#ifdef INT_UART_TX
unsigned char txBuffer[TX_BUFFER_SIZE];
volatile unsigned short indexTxBuffer = 0;
volatile unsigned short txBufferLength = 0;
#endif //#ifdef INT_UART_TX



/*--- low level functions ---*/

void UART_putc(unsigned char c)
{
#ifdef INT_UART_TX
        if(txBufferLength || U1STAbits.UTXBF) {         // Si la file contient une donnée ou que le buffer du périphérique est plein
                while (txBufferLength >= TX_BUFFER_SIZE);
                IEC0bits.U1TXIE = 0;    // Désactivation de l'interruption pour modifier les variables globales
                txBuffer[(indexTxBuffer + txBufferLength) % TX_BUFFER_SIZE] = c;
                txBufferLength ++;
                IEC0bits.U1TXIE = 1;    // Activation de l'interruption
                return;
        } else U1TXREG = c;
       
#else //#ifdef INT_UART_TX
       
        while (U1STAbits.UTXBF);
        U1TXREG = c;
#endif //#ifdef INT_UART_TX
       
        return;
}

void UART_send_tab(unsigned char *addr, unsigned char size) {
        addr += size;
        for (;size > 0; size --) {
                addr --;
                UART_putc(*addr);
        }
}

int UART_getc(unsigned char *byte) {
        if (rxBufferLength) {
                *byte = rxBuffer[indexRxBuffer];
                IEC0bits.U1RXIE = 0;    // Désactivation de l'interruption pour modifier les variables globales
                rxBufferLength --;
                indexRxBuffer ++; indexRxBuffer %= RX_BUFFER_SIZE;
                IEC0bits.U1RXIE = 1;    // Activation de l'interruption
                return 1;
        }
        return 0;
}      

void __attribute__((interrupt, auto_psv)) _U1RXInterrupt(void)
{
        IFS0bits.U1RXIF = 0; // On s'acquitte de l'interruption

        if(U1STAbits.FERR == 1) // Erreurs ?
                return ;
        /* must clear the overrun error to keep uart receiving */
        if(U1STAbits.OERR == 1)
        {
                U1STAbits.OERR = 0;
                return ;
        }

        /* get the data */
        if(U1STAbits.URXDA == 1) {
                rxBuffer[(indexRxBuffer + rxBufferLength) % RX_BUFFER_SIZE] = U1RXREG;
                rxBufferLength ++;
        }
        //if(U1STAbits.URXDA == 1) message_processing(U1RXREG);
}

/*void __attribute__((interrupt, auto_psv)) _U1ErrInterrupt(void)
{
        IFS4bits.U1EIF = 0; // On s'acquitte de l'interruption
        if(U1STAbits.FERR == 1) // Erreurs ?
                return ;
        // must clear the overrun error to keep uart receiving
        if(U1STAbits.OERR == 1)
                U1STAbits.OERR = 0;
        //error();

}*/

#ifdef INT_UART_TX
void __attribute__((interrupt, auto_psv)) _U1TXInterrupt(void)
{
        IFS0bits.U1TXIF = 0; // On s'acquitte de l'interruption
       
        if(U1STAbits.FERR == 1) // Erreurs ?
                return ;
        // must clear the overrun error to keep uart receiving
        if(U1STAbits.OERR == 1)
        {
                U1STAbits.OERR = 0;
                return ;
        }
       
        while (txBufferLength && !U1STAbits.UTXBF) {            // Si le buffer du module n'est pas plein
                U1TXREG = txBuffer[indexTxBuffer];
                txBufferLength --;
                indexTxBuffer ++; indexTxBuffer %= TX_BUFFER_SIZE;
        }
}
#endif //#ifdef INT_UART_TX
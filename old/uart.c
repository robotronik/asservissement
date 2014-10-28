/*---------------------------------------------------------------------------*
 * Couche basse uart                                                         *
 *---------------------------------------------------------------------------*/



/*------------------------------------ include ------------------------------*/


#include "main.h"
#include "message.h"

/*------------------------------------ variables and define -----------------*/

#ifdef INT_UART_TX
#define TX_BUFFER_SIZE 64
unsigned char txBuffer[TX_BUFFER_SIZE];
volatile short indexTxBuffer = 0;
volatile short txBufferLength = 0;
#endif //#ifdef INT_UART_TX

/*--------------------------------------------------------------------------------*/
void sendDone(void)
{
	UART_putc(DONE);
//	UART_putc('A');
}	


void sendDerapage(void)
{
    UART_putc(DERAPAGE);
}

/*------------------------------------ low level functions ------------------*/

int UART_putc(unsigned char c)
{
#ifdef INT_UART_TX
	if(U1STAbits.UTXBF) {		// Si le buffer du module est plein
		txBuffer[(indexTxBuffer + txBufferLength) % TX_BUFFER_SIZE] = c;
		txBufferLength ++;
IFS0bits.U1TXIF = 0;
IEC0bits.U1TXIE = 1;	// Activation de l'interruption sur l'envoie
		return -1;
	} else U1TXREG = c;

#else //#ifdef INT_UART_TX
	
	while (U1STAbits.UTXBF);
	U1TXREG = c;
#endif //#ifdef INT_UART_TX
	
	/*
	static short cnt;
	while (U1STAbits.UTXBF) cnt++;	// Si le buffer du module est plein
	U1TXREG = c;//*/
IEC0bits.U1TXIE = 0;	// Desactivation de l'interruption sur l'envoie
	return 0;
}

int send_long(long message) {
	if (UART_putc(message >> 24)) return 0;
	if (UART_putc(message >> 16)) return 1;
	if (UART_putc(message >> 8)) return 2;
	if (UART_putc(message)) return 3;
	return 4;
}

int send_short(short message) {
	if (UART_putc(message >> 8)) return 0;
	if (UART_putc(message)) return 1;
	return 2;
}


void __attribute__((interrupt, auto_psv)) _U1RXInterrupt(void)
{
	IFS0bits.U1RXIF = 0; // On s'acquitte de l'interruption

	if(U1STAbits.FERR == 1)	// Erreurs ?
		return ;
	/* must clear the overrun error to keep uart receiving */
	if(U1STAbits.OERR == 1)
	{
		U1STAbits.OERR = 0;
		return ;
	}

	/* get the data */
	if(U1STAbits.URXDA == 1) message_processing(U1RXREG);
}

/*void __attribute__((interrupt, auto_psv)) _U1ErrInterrupt(void)
{
	IFS4bits.U1EIF = 0; // On s'acquitte de l'interruption
	if(U1STAbits.FERR == 1)	// Erreurs ?
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
	
	if(U1STAbits.FERR == 1)	// Erreurs ?
		return ;
	// must clear the overrun error to keep uart receiving
	if(U1STAbits.OERR == 1)
	{
		U1STAbits.OERR = 0;
		return ;
	}
	
	/*if (txBufferSize) {
		U1TXREG = txBufferLength + 128;
		txBufferSize --;
		pause_s(1);
	}*/
	int i=0;
	while (txBufferLength && (i<4)) {
		U1TXREG = txBuffer[indexTxBuffer];
		txBufferLength --;
		indexTxBuffer ++; indexTxBuffer %= TX_BUFFER_SIZE;
		i ++;
	}
}
#endif //#ifdef INT_UART_TX

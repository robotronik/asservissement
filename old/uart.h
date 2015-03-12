/*---------------------------------------------------------------------------*
 * Couche basse uart                                                         *
 *---------------------------------------------------------------------------*/



#ifndef _UART_H
#define _UART_H

/* Functions headers */

#define DONE 0x10
#define ALPHA_GET 0xAA

int UART_putc(unsigned char c);
void __attribute__((interrupt, auto_psv)) _U1RXInterrupt(void);
void __attribute__((interrupt, auto_psv)) _U1TXInterrupt(void);

#endif

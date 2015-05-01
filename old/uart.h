/*---------------------------------------------------------------------------*
 * Couche basse uart                                                         *
 *---------------------------------------------------------------------------*/



#ifndef _UART_H
#define _UART_H

/* Functions headers */

#define DONE 0x10
#define ALPHA_GET 0xAA

int UART_putc(unsigned char c);

void sendDone(void);
void sendDerapage(void);

int send_long(long message);
int send_short(short message);

#endif

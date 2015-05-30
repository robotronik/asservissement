#ifndef HARDWARE_H
#define HARDWARE_H

/*void set_PWM_moteur_D(int PWM);
void set_PWM_moteur_G(int PWM);
int get_nbr_tick_D();
int get_nbr_tick_G();
int doit_attendre();*/

void init_hardware();
void set_PWM_moteur_D(int PWM);
void set_PWM_moteur_G(int PWM);
long int get_nbr_tick_D();
long int get_nbr_tick_G();
void attente_synchro();

void allumer_del();
void pause_ms(unsigned short n);
void eteindre_del();
void motors_stop();

int UART_getc(unsigned char *c);

int arret();

#endif

#define MAX_SPEED 3999//3999	// Entrée max des PWMs = 2*Période des PWMs
						// Fréquence des PWMs = 20 kHz
#define MIN_SPEED 900	// Entrée min sauf pour l'arrêt

void init(void);
void QEI_init ();
void timer_init();
void set_PWM_moteur_D(int PWM);
void set_PWM_moteur_G(int PWM);
void motors_stop(void); // Arrete les moteurs
int get_nbr_tick_D();
int get_nbr_tick_G();
int doit_attendre();
void allumer_del(void);
void eteindre_del(void);
void pause_ms(unsigned short n);
void pause_s(unsigned short n);
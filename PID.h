
#define Kp_delta //constante de proportionalité pour delta
#define Ki_delta //composante d'intégration pour delta
#define Kd_delta //composante de dérivation pour delta
// Alpha
#define Kp_alpha //constante de proportionalité
#define Ki_alpha //composante d'intégration
#define Kd_alpha //composante de dérivation

int PID_lineique(int erreur, int erreur_preced, int erreur_sum);
int PID_angulaire(int erreur, int erreur_preced, int erreur_sum);
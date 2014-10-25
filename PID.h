
#define KD_DELTA //constante de proportionalité pour delta
#define KI_DELTA //composante d'intégration pour delta
#define KD_DELTA //composante de dérivation pour delta
// Alpha
#define KD_ALPHA //constante de proportionalité
#define KI_ALPHA //composante d'intégration
#define KD_ALPHA //composante de dérivation

int PID_lineique(int erreur, int erreur_preced, int erreur_sum);
int PID_angulaire(int erreur, int erreur_preced, int erreur_sum);
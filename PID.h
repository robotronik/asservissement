
#define KD_DELTA 1//constante de proportionalité pour delta
#define KI_DELTA 1//composante d'intégration pour delta
#define KD_DELTA 1//composante de dérivation pour delta
// Alpha
#define KD_ALPHA 1//constante de proportionalité
#define KI_ALPHA 1//composante d'intégration
#define KD_ALPHA 1//composante de dérivation

int PID_lineique(int erreur, int erreur_preced, int erreur_sum);
int PID_angulaire(int erreur, int erreur_preced, int erreur_sum);
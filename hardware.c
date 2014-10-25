//ici la couche d'abstraction du microcontroleur utilisé
//on fait ceci afin de rendre le code portable

void set_PWM_moteur_D(int PWM)
{}

void set_PWM_moteur_G(int PWM)
{}

int get_nbr_tick_D()
{}

int get_nbr_tick_G()
{}

int doit_attendre()
{
	//synchronisation de asser() à une fréquence régulière
}

//ajouter fonction d'envoi et de réception par l'uart 
//une fois quelque chose reçu on envoie à "communicution.c"
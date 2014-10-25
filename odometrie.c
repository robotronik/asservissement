//ici les calculs de position actuelle

//oh les vilaines variables globales !
int x_actuel;
int y_actuel;
int a_actuel;
int delta_actuel;
int alpha_actuel; //relatif
int tetha_actuel; //absolu

void init_odometrie()
{
	x_actuel=0;
	y_actuel=0;
	a_actuel=0;
	delta_actuel=0;
	alpha_actuel=0;
	tetha_actuel=0;
}

void set_delta_actuel(int delta)
{
	delta_actuel=delta;
}
void set_alpha_actuel(int alpha)
{
	alpha_actuel=alpha;
}

int get_delta_actuel()
{
	return delta_actuel;
}
int get_alpha_actuel()
{
	return alpha_actuel;
}

int get_tetha_actuel()
{
	return tetha_actuel;
}

int get_x_actuel()
{
	return x_actuel;
}
int get_y_actuel()
{
	return y_actuel;
}

void actualise_position(nbr_tick_D,nbr_tick_G)
{
	//actualisation de x_actuel,y_actuel,a_actuel,delta_actuel,alpha_actuel,tetha_actuel
}
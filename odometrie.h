void init_odometrie();

void set_delta_actuel(int delta);
void set_alpha_actuel(int alpha);

int get_delta_actuel();
int get_alpha_actuel();
int get_tetha_actuel();
int get_x_actuel();
int get_y_actuel();

void actualise_position(int nbr_tick_D, int nbr_tick_G);
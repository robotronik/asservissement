// Convertir les milliradians en degrés pour l'affichage
#define MRAD2DEGRES 0.0572957795131
// Dimensions
// Le facteur de zoom (mm par pixel)
#define ZOOM_FACTOR 5

#define PLATEAU_HEIGHT 2000
#define PLATEAU_WIDTH 3000

#define ROBOT_HEIGHT 250
#define ROBOT_WIDTH  230

void bouge_robot_sdl(int x, int y, int alpha);
int sdl_manage_events();
int init_sdl_screen();
int quit_sdl_screen();

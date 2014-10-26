typedef int bool;
#define true 1
#define false 0

// Dimensions
// Le facteur de zoom (mm par pixel)
#define ZOOM_FACTOR 5

#define HEIGHT 2000.
#define WIDTH 3000.

#define ROBOT_HEIGHT 300.
#define ROBOT_WIDTH  300.

int set_position(int x, int y, float alpha);
void dessine_robot();

int sdl_manage_events();
int init_sdl_screen();
int quit_sdl_screen();
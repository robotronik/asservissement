#include "asser.h"
#include "SDL/affichage.h"

int main()
{
    if (init_sdl_screen() < 0)
        return 1;

    int i=1;
    set_position(100, 100, 45);

    
	asser();
	return quit_sdl_screen();
}
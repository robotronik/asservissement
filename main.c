#include "asser.h"
#include "debug/affichage.h"

int main()
{
    if (init_sdl_screen() < 0)
        return 1;

    int i=1;


	asser();
	return quit_sdl_screen();
}
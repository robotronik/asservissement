#include "asser.h"
#include "trajectoire.h"
#include "odometrie.h"
#include "debug/affichage.h"

int main()
{
    if (init_sdl_screen() < 0)
        return 1;

    int i=1;

    init_odometrie();
    init_trajectoire();
	asser();
	return quit_sdl_screen();
}
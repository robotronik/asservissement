#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "affichage.h"

SDL_Surface* fenetre = 0;
SDL_Event evenements = {0};
SDL_Surface* image_robot;
SDL_Rect position_robot;


int main(int argc, char const *argv[]) {
    if (init_sdl_screen() < 0)
        return 1;

    int i=1;

    while(!sdl_manage_events()) {
        if (i < 300){
            i+=1;
            position_robot.x = i;
            position_robot.y = i;
            // Remplissage de la surface avec du blanc
            SDL_FillRect(fenetre, NULL, SDL_MapRGB(fenetre->format, 255, 255, 255));
            SDL_BlitSurface(image_robot, NULL, fenetre, &position_robot); // Collage de la surface sur l'écran

            SDL_Flip(fenetre); // Mise à jour de l'écran
        }
    }

    return quit_sdl_screen();
}



int sdl_manage_events() {
    SDL_PollEvent(&evenements);
    if(evenements.type == SDL_QUIT)
        return true;
    else
        return false;
}

int init_sdl_screen() { 
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
        return quit_sdl_screen(1);

    fenetre = SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    SDL_WM_SetCaption("SDL : Une p'tite gestion de l'asservissement", NULL);

    if (fenetre == 0)
        return quit_sdl_screen(1);

    image_robot = IMG_Load("robot.png");

}

int quit_sdl_screen(int erreur) {
    if (erreur != 0)
        printf("Erreur lors de la creation de la fenetre : ,%s",SDL_GetError());

    SDL_Quit();
    return -1;
}

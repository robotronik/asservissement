#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_rotozoom.h>
#include "affichage.h"

SDL_Surface* fenetre = 0;
SDL_Event evenements = {0};
SDL_Surface* image_robot;
SDL_Rect position_robot;

int set_position(int x, int y, float alpha) {
    position_robot.x = x;
    position_robot.y = y;
    // Remplissage de la surface avec du blanc
    SDL_FillRect(fenetre, NULL, SDL_MapRGB(fenetre->format, 255, 255, 255));

    // Collage de l'image du robot, rotaté, à la position qu'il faut.
    SDL_BlitSurface(rotozoomSurface(image_robot, alpha, 1.0, 1),
        NULL, fenetre, &position_robot);

    SDL_Flip(fenetre); // Mise à jour de l'écran
    sdl_manage_events(); // On gère les événements qui ont apparus, au cas où
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

    fenetre = SDL_SetVideoMode(600, 400, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    SDL_WM_SetCaption("SDL : Une p'tite gestion de l'asservissement", NULL);

    if (fenetre == 0)
        return quit_sdl_screen(1);

    SDL_FillRect(fenetre, NULL, SDL_MapRGB(fenetre->format, 255, 255, 255));
    image_robot = IMG_Load("SDL/robot.png");
}

int quit_sdl_screen(int erreur) {
    if (erreur != 0)
        printf("Erreur lors de la creation de la fenetre : ,%s",SDL_GetError());

    SDL_Quit();
    return -1;
}

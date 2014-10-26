#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <png.h>
#include "affichage.h"

SDL_Event evenements = {0};
SDL_Surface* image_robot;
SDL_Rect position_robot;

float robot_width, robot_height;

int set_position(int x, int y, float alpha) {
    float position_x = 2*x/WIDTH - 1,
        position_y =  - 2*y/HEIGHT + 1;

    // Conversion de position vers position des vertices (avec centrage)
    printf("x : %f, y : %f\n", position_x, position_y);

    // Remplissage de la surface avec du noir
    glClear(GL_COLOR_BUFFER_BIT);      
    glBegin(GL_TRIANGLES);
        glColor3ub(0,255,255);
        glVertex2d(position_x - robot_width/2, position_y + robot_height/2);
        glVertex2d(position_x - robot_width/2, position_y - robot_height/2);
        glVertex2d(position_x + robot_width/2, position_y);
    glEnd();

    // Collage de l'image du robot, rotaté, à la position qu'il faut.
    //SDL_BlitSurface(rotozoomSurface(image_robot, alpha, 1.0, 1),
    //    NULL, fenetre, &position_robot);

    glFlush();
    SDL_GL_SwapBuffers(); // Mise à jour de l'écran
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

    SDL_WM_SetCaption("SDL : Une p'tite gestion de l'asservissement", NULL);

    if (SDL_SetVideoMode(WIDTH/ZOOM_FACTOR, HEIGHT/ZOOM_FACTOR, 32, SDL_OPENGL) == 0)
        return quit_sdl_screen(1);


    robot_width = ROBOT_WIDTH/WIDTH;
    robot_height = ROBOT_HEIGHT/HEIGHT;

    image_robot = IMG_Load("robot.png");
}

int quit_sdl_screen(int erreur) {
    if (erreur != 0)
        printf("Erreur lors de la creation de la fenetre : ,%s",SDL_GetError());

    SDL_Quit();
    return -1;
}

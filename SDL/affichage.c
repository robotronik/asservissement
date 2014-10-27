#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SOIL/SOIL.h>
#include "affichage.h"

#define AFFICHAGE_DEBUG 1

SDL_Event evenements;
GLuint texturePlateau;

SDL_Rect position_robot;

float robot_width, robot_height,
    plateau_width, plateau_height;


void dessine_robot() {
    glDisable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
        glColor3ub(0,0,255);
        glVertex2d(-robot_width/2, -robot_height/2);
        glVertex2d(-robot_width/2, +robot_height/2);
        glVertex2d(+robot_width/2, +robot_height/2);
        glVertex2d(+robot_width/2, -robot_height/2);
    glEnd();
    glBegin(GL_TRIANGLES);
        glVertex2d(-robot_width/2, -robot_height/2);
        glVertex2d(+robot_width/2, -robot_height/2);
        glColor3ub(255,0,0);
        glVertex2d(0, robot_height/2);
    glEnd();
}

void dessine_fond() {
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
        glColor3ub(255,255,255);
        glTexCoord2d (0, 0); glVertex2d(0,              0);
        glTexCoord2d (0, 1); glVertex2d(0,              plateau_height);
        glTexCoord2d (1, 1); glVertex2d(plateau_width,  plateau_height);
        glTexCoord2d (1, 0); glVertex2d(plateau_width,  0);
    glEnd();
}

int set_position(int x, int y, float alpha) {
    float position_x = 2*x/ZOOM_FACTOR+200,
        position_y =  2*y/ZOOM_FACTOR;

    if (AFFICHAGE_DEBUG == 1)
        printf("x = %d, position_x = %f\ny = %d, position_y = %f\nalpha = %f\n",
                x,      position_x,      y,      position_y,      alpha);

    // Test values
    // alpha = 180;

    // Remplissage de la surface avec du noir
    glClear(GL_COLOR_BUFFER_BIT);    
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );
    dessine_fond();
    glTranslated(position_x, position_y, 0);
    glRotatef(alpha, 0, 0, 1);
    dessine_robot();

    glFlush();
    SDL_GL_SwapBuffers(); // Mise à jour de l'écran
    sdl_manage_events(); // On gère les événements qui sont apparus, au cas où
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

    // Changer de repère : repère orthogonal avec origine bas-gauche
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );
    gluOrtho2D(0,WIDTH/ZOOM_FACTOR,0,HEIGHT/ZOOM_FACTOR);

    // Texture : plateau de jeu
    texturePlateau = SOIL_load_OGL_texture("SDL/plateau.png",
        SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    printf("SOIL messages : '%s' (SDL/plateau.png)\n", SOIL_last_result());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, texturePlateau);

    // Calcul des dimensions
    plateau_width = WIDTH /ZOOM_FACTOR;
    plateau_height= HEIGHT/ZOOM_FACTOR;

    robot_width = ROBOT_WIDTH/ZOOM_FACTOR;
    robot_height= ROBOT_HEIGHT/ZOOM_FACTOR;
}

int quit_sdl_screen(int erreur) {
    if (erreur != 0)
        printf("Erreur lors de la creation de la fenetre : ,%s",SDL_GetError());

    SDL_Quit();
    return -1;
}

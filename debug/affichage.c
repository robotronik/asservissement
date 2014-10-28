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
        glColor3ub(255,0,200);
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

void set_position(int x, int y, int alpha) {
    float alpha_deg = alpha * MRAD2DEGRES;
    if (AFFICHAGE_DEBUG == 1)
        printf("x = %d, y = %d, alpha = %f\n",
                x,      y,      alpha_deg);

    // Remplissage de la surface avec du noir
    glClear(GL_COLOR_BUFFER_BIT);    
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );
    dessine_fond();
    glTranslated(x, y, 0);
    glRotatef(alpha_deg, 0, 0, 1);
    dessine_robot();

    glFlush();
    SDL_GL_SwapBuffers(); // Mise à jour de l'écran
    sdl_manage_events(); // On gère les événements qui sont apparus, au cas où
}



int sdl_manage_events() {
    SDL_PollEvent(&evenements);
    if(evenements.type == SDL_QUIT)
        return 1;
    else
        return 0;
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
    gluOrtho2D(0,WIDTH,0,HEIGHT);

    // Texture : plateau de jeu
    texturePlateau = SOIL_load_OGL_texture("plateau.png",
        SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    if (AFFICHAGE_DEBUG == 1 || texturePlateau == 0)
        printf("SOIL messages : '%s' (plateau.png)\n", SOIL_last_result());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, texturePlateau);

    // Calcul des dimensions
    plateau_width = WIDTH ;
    plateau_height= HEIGHT;

    robot_width = ROBOT_WIDTH;
    robot_height= ROBOT_HEIGHT;
    return 0;
}

int quit_sdl_screen(int erreur) {
    if (erreur != 0)
        printf("Erreur lors de la creation de la fenetre : ,%s",SDL_GetError());

    SDL_Quit();
    return -1;
}

#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SOIL/SOIL.h>
#include "affichage.h"
#include "../communication.h"



SDL_Event evenements;
GLuint texturePlateau;

SDL_Rect position_robot;


int souris_x, souris_y;


void dessine_robot() {
    glDisable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
        glColor3ub(0,0,255);
        glVertex2d(-ROBOT_WIDTH/2, -ROBOT_HEIGHT/2);
        glVertex2d(-ROBOT_WIDTH/2, +ROBOT_HEIGHT/2);
        glVertex2d(+ROBOT_WIDTH/2, +ROBOT_HEIGHT/2);
        glVertex2d(+ROBOT_WIDTH/2, -ROBOT_HEIGHT/2);
    glEnd();
    glBegin(GL_TRIANGLES);
        glColor3ub(0,0,255);
        glVertex2d(-ROBOT_WIDTH/2, -ROBOT_HEIGHT/2);
        glVertex2d(+ROBOT_WIDTH/2, -ROBOT_HEIGHT/2);
        glColor3ub(255,0,200);
        glVertex2d(0, ROBOT_HEIGHT/2);
    glEnd();
    glBegin(GL_TRIANGLES);
        glColor3ub(0,0,255);
        glVertex2d(-ROBOT_WIDTH/2, -ROBOT_HEIGHT/2);
        glVertex2d(+ROBOT_WIDTH/2, -ROBOT_HEIGHT/2);
        //glColor3ub(0,0,0);
        glVertex2d(0, 0);
    glEnd();
}

void dessine_fond() {
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
        glColor3ub(255,255,255);
        glTexCoord2d (0, 0); glVertex2d(0,              0);
        glTexCoord2d (0, 1); glVertex2d(0,              PLATEAU_HEIGHT);
        glTexCoord2d (1, 1); glVertex2d(PLATEAU_WIDTH,  PLATEAU_HEIGHT);
        glTexCoord2d (1, 0); glVertex2d(PLATEAU_WIDTH,  0);
    glEnd();
}

void bouge_robot_sdl(int x, int y, int alpha_mrad) {
    set_position(x, y, alpha_mrad);
    //sdl_manage_events(); // On gère les événements qui sont apparus, au cas où
}

void set_position(int x, int y, int alpha) {
    float alpha_deg = alpha * MRAD2DEGRES;
#if DEBUG
    printf("x = %d, y = %d, alpha = %f\n",
            x,      y,      alpha_deg);
#endif
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
    switch(evenements.type) {
        case SDL_QUIT:
            return 1;
        case SDL_MOUSEBUTTONDOWN: case SDL_MOUSEMOTION: /* Clic de la souris */
            if (evenements.button.button == SDL_BUTTON_LEFT) {
#if DEBUG
                printf("%d %d\n", evenements.button.x*ZOOM_FACTOR, PLATEAU_HEIGHT - evenements.button.y*ZOOM_FACTOR);
#endif
                new_xy_absolu(evenements.button.x*ZOOM_FACTOR, PLATEAU_HEIGHT - evenements.button.y*ZOOM_FACTOR);
                return 0;
            } else
                return 0;
            break;
        default:
            return 0;
    }
}

int init_sdl_screen() {
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
        return quit_sdl_screen(1);

    SDL_WM_SetCaption("SDL : Une p'tite gestion de l'asservissement", NULL);

    if (SDL_SetVideoMode(PLATEAU_WIDTH/ZOOM_FACTOR+1, PLATEAU_HEIGHT/ZOOM_FACTOR+1, 32, SDL_OPENGL) == 0)
        return quit_sdl_screen(1);

    // Changer de repère : repère orthogonal avec origine bas-gauche
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );
    gluOrtho2D(0, PLATEAU_WIDTH, 0, PLATEAU_HEIGHT);

    // Texture : plateau de jeu
    texturePlateau = SOIL_load_OGL_texture("plateau.png",
        SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
#if DEBUG
    printf("SOIL messages : '%s' (plateau.png)\n", SOIL_last_result());
#endif
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, texturePlateau);

    return 0;
}

int quit_sdl_screen(int erreur) {
    if (erreur != 0)
        printf("Erreur lors de la creation de la fenetre : ,%s",SDL_GetError());

    SDL_Quit();
    return -1;
}

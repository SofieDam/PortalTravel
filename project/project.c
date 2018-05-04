#ifdef __APPLE__
#include <OpenGL/gl3.h>
	// Linking hint for Lightweight IDE
	// uses framework Cocoa
#endif
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"
#include <math.h>

#include "cube.h"
#include "keyboard.c"
#include "help_functions.c"
#include "menu/menu.c"
#include "skybox/skybox.c"
#include "islandWorld/island.c"
#include "forestWorld/forest.c"

int screen_size = 800;

// Decides which world we will draw
int world = 1;                  // {0,1,2}


void timer(int i)
{
    glutTimerFunc(20, &timer, i);
    glutPostRedisplay();
}

void init(void)
{
    // GL inits
    //glClearColor(0.6, 0.6, 0.9, 0);
    glClearColor(0.0, 0.0, 0.0, 0);

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    initMenu(screen_size, world);
    initSkybox();
    initIslandWorld();
    initForestWorld();
}

void display(void)
{
    // clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    getWorld(&world);


    // Main menu
    if (world == 0)
    {
        displayMenu();
    }

    // Island world
    else if (world == 1)
    {
        displayIslandWorld();
        displayPortalMenu_forestWorld();
    }

    // Forest world
    else if (world == 2)
    {
        displayForestWorld();
        displayPortalMenu_islandWorld();
    }

    // Error
    else
    {
        printf("World is not less than 3 \n");
        exit(0);
    }

    glutSwapBuffers();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
    glutInitContextVersion(3, 2);
    glutInitWindowSize (screen_size, screen_size);
    glutCreateWindow ("TSBK07 project");

    glutDisplayFunc(display);
    init ();
    glutTimerFunc(20, &timer, 0);

    glutPassiveMotionFunc(mouseOverListener);
    glutMouseFunc(mouseEvent);
    glutKeyboardFunc(keyPressed);


    glutMainLoop();

    exit(0);
}

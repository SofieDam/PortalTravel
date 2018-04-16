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

#include "keyboard.c"
#include "islandWorld/island.c"
#include "forestWorld/forest.c"


int world = 0;      // Decides which world we will draw


void timer(int i)
{
    glutTimerFunc(20, &timer, i);
    glutPostRedisplay();
}


void init(void)
{
    // GL inits
    //glClearColor(0.2,0.5,0.8,0);
    glClearColor(0.6, 0.6, 0.9, 0);
    glEnable(GL_DEPTH_TEST);
    //glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    printError("GL inits");

    initIslandWorld();
    initForestWorld();
}

void display(void)
{
    if (world == 0) {
        displayIslandWorld();
    }

    glutSwapBuffers();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
    glutInitContextVersion(3, 2);
    glutInitWindowSize (700, 700);
    glutCreateWindow ("TSBK07 project");
    glutDisplayFunc(display);
    init ();
    glutTimerFunc(20, &timer, 0);

    glutKeyboardFunc(keyPressed);

    glutMainLoop();
    exit(0);
}
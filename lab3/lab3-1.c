#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	// Linking hint for Lightweight IDE
	// uses framework Cocoa
#endif
#include "MicroGlut.h"
#include "GL_utilities.h"
#include <math.h>
#include "loadobj.h"
#include "LoadTGA.h"
#include "VectorUtils3.h"

#define near 1.0
#define far 30.0
#define right 0.5
#define left -0.5
#define top 0.5
#define bottom -0.5

// Modified projection matrix, see p.59 in course book
GLfloat projectionMatrix[] = {    2.0f*near/(right-left), 0.0f, (right+left)/(right-left), 0.0f,
								  0.0f, 2.0f*near/(top-bottom), (top+bottom)/(top-bottom), 0.0f,
								  0.0f, 0.0f, -(far + near)/(far - near), -2*far*near/(far - near),
								  0.0f, 0.0f, -1.0f, 0.0f };


// Reference to shader program
GLuint program;

// lab3-1
// Page 187 in the coursebook
struct GraphicsEntity
{
    char *name;
    Model *m;
    mat4 translation;
    mat4 rotation;
    mat4 scale;

    struct GraphicsEntity *child;
    struct GraphicsEntity *next;

};

struct GraphicsEntity Windmill;


void OnTimer(int value)
{
	glutPostRedisplay();
	glutTimerFunc(20, &OnTimer, value);
}


void initWindmill(){

    Windmill.name = strdup("windmill");
    Windmill.m = LoadModelPlus("windmill/windmill-walls.obj");
    Windmill.translation = T(0, -1.5, 0);
    Windmill.rotation = Ry(0);
    Windmill.scale = S(0.2, 0.2, 0.2);

    Windmill.child = NULL;
    Windmill.next = NULL;

    printError("init windmill");
}


void init(void)
{
	dumpInfo();

	// Assignment: Change the color of the background.
	glClearColor(0.4,0.6,0.8,0);

	// Turn on Z-buffer
	glEnable(GL_DEPTH_TEST);
	//glDisable(GL_DEPTH_TEST);
    printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab3-1.vert", "lab3-1.frag");
	printError("init shader");

    // lab3-1
    initWindmill();
    printError("initWindmill()");

}

void draw(struct GraphicsEntity entity)
{
    /*
 * void gluLookAt(GLdouble eyeX,  GLdouble eyeY,  GLdouble eyeZ,
 * 				  GLdouble centerX,  GLdouble centerY,  GLdouble centerZ,
 * 				  GLdouble upX,  GLdouble upY,  GLdouble upZ);
 */
    mat4 worldToView = lookAt(0,0,8, 0,0,0, 0,1,0);

    mat4 total = Mult(Mult(entity.translation, entity.rotation), entity.scale);

    glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
    glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, worldToView.m);
    glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);

    DrawModel(entity.m, program, "in_Position", "in_Normal", NULL);

}

void display(void)
{
	printError("pre display");

	// Clear the screen and Z-buffer
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

    // lab3-1
    draw(Windmill);

	glutSwapBuffers();
}


int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitContextVersion(3, 2);

	// Set up Z-buffer
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutInitWindowSize (500, 500);
	glutCreateWindow ("lab3-1");
	glutDisplayFunc(display);
	init ();

    // lab1-3
    glutTimerFunc(20, &OnTimer, 0);

	glutMainLoop();
	return 0;
}

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


Model *bunny, *teapot;

GLfloat a = 0;

// Reference to shader program
GLuint program;


void OnTimer(int value)
{
	glutPostRedisplay();
	glutTimerFunc(20, &OnTimer, value);
}


void init(void)
{
	dumpInfo();

	// Assignment: Change the color of the background.
	glClearColor(0.3,0.3,0.5,0);

	// Turn on Z-buffer
	glEnable(GL_DEPTH_TEST);
	//glDisable(GL_DEPTH_TEST);

	printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab2-7.vert", "lab2-7.frag");
	printError("init shader");

	// lab2-7
    // Upload geometry to the GPU:
    bunny = LoadModelPlus("bunnyplus.obj");
    teapot =  LoadModelPlus("teapot.obj");

}


void display(void)
{
	printError("pre display");

	// Clear the screen and Z-buffer
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

	mat4 rot, trans, scale, total, worldToView;

	a += 0.01;

	// lab2-7

	/*
	 * void gluLookAt(GLdouble eyeX,  GLdouble eyeY,  GLdouble eyeZ,
	 * 				  GLdouble centerX,  GLdouble centerY,  GLdouble centerZ,
	 * 				  GLdouble upX,  GLdouble upY,  GLdouble upZ);
	 */
    worldToView = lookAt(4*cos(a),0,4*sin(a), 0,0,0, 0,1,0);


    // bunny
    rot = Ry(0);
    trans = T(0.6, 0, 0);
    scale = S(0.8,0.8,0.8);
    total = Mult(Mult(rot, trans),scale);
    glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
    glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, worldToView.m);
    glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(bunny, program, "in_Position", "in_Normal", NULL);

    // teapot
    rot = Ry(0);
    trans = T(-0.6, -0.5, 0);
    scale = S(0.1,0.1,0.1);
    total = Mult(Mult(rot, trans),scale);
    glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
    glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, worldToView.m);
    glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(teapot, program, "in_Position", "in_Normal", NULL);

	printError("display");

	glutSwapBuffers();
}


int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitContextVersion(3, 2);

	// Set up Z-buffer
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutInitWindowSize (500, 500);
	glutCreateWindow ("lab2-7");
	glutDisplayFunc(display);
	init ();

    // lab1-3
    glutTimerFunc(20, &OnTimer, 0);

	glutMainLoop();
	return 0;
}

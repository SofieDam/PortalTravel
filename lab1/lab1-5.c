// Should work as is on Linux and Mac. MS Windows needs GLEW or glee.
// See separate Visual Studio version of my demos.
#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	// Linking hint for Lightweight IDE
	// uses framework Cocoa
#endif
#include "MicroGlut.h"
#include "GL_utilities.h"

// lab1-3
#include <math.h>

// Globals
// Data would normally be read from files
GLfloat cube[36*3] = {
		-0.5,-0.5,-0.5,
		-0.5,0.5,-0.5,
		0.5,0.5,-0.5,
		-0.5,-0.5,-0.5,
		0.5,0.5,-0.5,
		0.5,-0.5,-0.5,

		0.5,0.5,-0.5,
		-0.5,0.5,-0.5,
		-0.5,0.5,0.5,
		0.5,0.5,-0.5,
		-0.5,0.5,0.5,
		0.5,0.5,0.5,

		-0.5,-0.5,-0.5,
		-0.5,-0.5,0.5,
		-0.5,0.5,0.5,
		-0.5,-0.5,-0.5,
		-0.5,0.5,0.5,
		-0.5,0.5,-0.5,

		0.5,-0.5,-0.5,
		0.5,0.5,-0.5,
		0.5,0.5,0.5,
		0.5,-0.5,-0.5,
		0.5,0.5,0.5,
		0.5,-0.5,0.5,

		-0.5,-0.5,0.5,
		0.5,-0.5,0.5,
		0.5,0.5,0.5,
		-0.5,-0.5,0.5,
		0.5,0.5,0.5,
		-0.5,0.5,0.5,

		-0.5,-0.5,-0.5,
		0.5,-0.5,-0.5,
		0.5,-0.5,0.5,
		-0.5,-0.5,-0.5,
		0.5,-0.5,0.5,
		-0.5,-0.5,0.5
};

// lab1-4
GLfloat cubeColor[36*3] = {
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0,
		1.0, 1.0, 0.0,
		1.0, 0.0, 1.0,
		0.0, 1.0, 1.0,

		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0,
		1.0, 1.0, 0.0,
		1.0, 0.0, 1.0,
		0.0, 1.0, 1.0,

		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0,
		1.0, 1.0, 0.0,
		1.0, 0.0, 1.0,
		0.0, 1.0, 1.0,

		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0,
		1.0, 1.0, 0.0,
		1.0, 0.0, 1.0,
		0.0, 1.0, 1.0,

		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0,
		1.0, 1.0, 0.0,
		1.0, 0.0, 1.0,
		0.0, 1.0, 1.0,

		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0,
		1.0, 1.0, 0.0,
		1.0, 0.0, 1.0,
		0.0, 1.0, 1.0
};

// vertex array object
unsigned int vertexArrayObjID;


// lab1-3
GLfloat a = 0;
// Reference to shader program
GLuint program;


// lab1-3
void OnTimer(int value)
{
	glutPostRedisplay();
	glutTimerFunc(20, &OnTimer, value);
}


void init(void)
{
	// vertex buffer object, used for uploading the geometry
	unsigned int vertexBufferObjID;

	// Assignment: Change the color of the triangle.
	unsigned int triangleColorBufferObjID;

	// Reference to shader program
	// GLuint program;

	dumpInfo();

	// GL inits
	//glClearColor(0.2,0.2,0.5,0);

	// Assignment: Change the color of the background.
	glClearColor(0.0,0.0,0.0,0);

	// lab1-5: turn on Z-buffer
	glEnable(GL_DEPTH_TEST);
	//glDisable(GL_DEPTH_TEST);

	printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab1-5.vert", "lab1-5.frag");
	printError("init shader");

	// Upload geometry to the GPU:

	// Allocate and activate Vertex Array Object
	glGenVertexArrays(1, &vertexArrayObjID);
	glBindVertexArray(vertexArrayObjID);
	// Allocate Vertex Buffer Objects
	glGenBuffers(1, &vertexBufferObjID);

	// Assignment lab1-1: Change the color of the triangle.
	glGenBuffers(1, &triangleColorBufferObjID);


	// VBO for vertex data
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, 36*3*sizeof(GLfloat), cube, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));

	// lab1-4
	// VBO for triangle color
	glBindBuffer(GL_ARRAY_BUFFER, triangleColorBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, 36*3*sizeof(GLfloat), cubeColor, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_Triangle_Color"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Triangle_Color"));

	// Assignment lab1-2
	//glUniformMatrix4fv(glGetUniformLocation(program, "myMatrix"), 1, GL_TRUE, myMatrix);

	// End of upload of geometry

	printError("init arrays");
}


void display(void)
{
	printError("pre display");

	// lab1-5: clear the screen and Z-buffer
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

    // lab1-5
    a += 0.01;

    // lab1-5
	GLfloat rotationMatrixX[] =
	{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, cos(a), -sin(a), 0.0f,
		0.0f, sin(a), cos(a), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	// lab1-5
	GLfloat rotationMatrixY[] =
	{
		cos(a), 0.0f, sin(a), 0.0f,
		 0.0f, 1.0f, 0.0f, 0.0f,
		 -sin(a), 0.0f, cos(a), 0.0f,
		 0.0f, 0.0f, 0.0f, 1.0f
	};

	// lab1-5
	GLfloat rotationMatrixZ[] =
	{
		cos(a), -sin(a), 0.0f, 0.0f,
		sin(a), cos(a), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};


    // lab1-5
	glUniformMatrix4fv(glGetUniformLocation(program, "myMatrixX"), 1, GL_TRUE, rotationMatrixX);
    glUniformMatrix4fv(glGetUniformLocation(program, "myMatrixY"), 1, GL_TRUE, rotationMatrixY);
	glUniformMatrix4fv(glGetUniformLocation(program, "myMatrixZ"), 1, GL_TRUE, rotationMatrixZ);


	glBindVertexArray(vertexArrayObjID);	// Select VAO
	glDrawArrays(GL_TRIANGLES, 0, 36*3);	// draw object

	printError("display");

	glutSwapBuffers();
}


int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitContextVersion(3, 2);

	// lab1-5: set up Z-buffer
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutInitWindowSize (500, 500);
	glutCreateWindow ("lab1-4");
	glutDisplayFunc(display);
	init ();

    // lab1-3
    glutTimerFunc(20, &OnTimer, 0);

	glutMainLoop();
	return 0;
}
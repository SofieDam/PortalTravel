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

// lab1-6
#include "loadobj.h"



// vertex array object
unsigned int vertexArrayObjID;

// lab1-6
unsigned int bunnyVertexArrayObjID;
Model *m;


// lab1-3
GLfloat a = 0;
GLfloat b = 0;
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

    // lab1-6
    unsigned int bunnyVertexBufferObjID;
    unsigned int bunnyIndexBufferObjID;
    unsigned int bunnyNormalBufferObjID;
    m = LoadModel("bunny.obj");

	dumpInfo();

	// GL inits
	//glClearColor(0.2,0.2,0.5,0);

	// Assignment: Change the color of the background.
	glClearColor(0.3,0.3,0.5,0);

	// lab1-5: turn on Z-buffer
	glEnable(GL_DEPTH_TEST);

	//glDisable(GL_DEPTH_TEST);
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab1-6.vert", "lab1-6.frag");
	printError("init shader");


    // lab1-6
    glGenVertexArrays(1, &bunnyVertexArrayObjID);
    glGenBuffers(1, &bunnyVertexBufferObjID);
    glGenBuffers(1, &bunnyIndexBufferObjID);
    glGenBuffers(1, &bunnyNormalBufferObjID);
    glBindVertexArray(bunnyVertexArrayObjID);


    // lab1-6: VBO for vertex data
    glBindBuffer(GL_ARRAY_BUFFER, bunnyVertexBufferObjID);
    glBufferData(GL_ARRAY_BUFFER, m->numVertices*3*sizeof(GLfloat), m->vertexArray, GL_STATIC_DRAW);
    glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));
    printError("init vertices");

    // lab1-6: VBO for normal data
    glBindBuffer(GL_ARRAY_BUFFER, bunnyNormalBufferObjID);
    glBufferData(GL_ARRAY_BUFFER, m->numVertices*3*sizeof(GLfloat), m->normalArray, GL_STATIC_DRAW);
    glVertexAttribPointer(glGetAttribLocation(program, "in_Normal"), 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(glGetAttribLocation(program, "in_Normal"));
    printError("init normals");

    // lab1-6: VBO for index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bunnyIndexBufferObjID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m->numIndices*sizeof(GLuint), m->indexArray, GL_STATIC_DRAW);
    printError("init index");

	// End of upload of geometry
}


void display(void)
{
	printError("pre display");

	// lab1-5: clear the screen and Z-buffer
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

    // lab1-5
    a += 0.01;
    b += 0.02;

    // lab1-5
	GLfloat rotationMatrixX[] =
	{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, cos(b), -sin(b), 0.0f,
		0.0f, sin(b), cos(b), 0.0f,
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
		cos(b), -sin(b), 0.0f, 0.0f,
		sin(b), cos(b), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};


    // lab1-5
	glUniformMatrix4fv(glGetUniformLocation(program, "myMatrixX"), 1, GL_TRUE, rotationMatrixX);
    glUniformMatrix4fv(glGetUniformLocation(program, "myMatrixY"), 1, GL_TRUE, rotationMatrixY);
	glUniformMatrix4fv(glGetUniformLocation(program, "myMatrixZ"), 1, GL_TRUE, rotationMatrixZ);


	//glBindVertexArray(vertexArrayObjID);	// Select VAO

    // lab1-6
    glBindVertexArray(bunnyVertexArrayObjID);
    glDrawElements(GL_TRIANGLES, m->numIndices, GL_UNSIGNED_INT, 0L);

	//glDrawArrays(GL_TRIANGLES, 0, 36*3);	// draw object

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
	glutCreateWindow ("lab1-6");
	glutDisplayFunc(display);
	init ();

    // lab1-3
    glutTimerFunc(20, &OnTimer, 0);

	glutMainLoop();
	return 0;
}

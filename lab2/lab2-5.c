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


// vertex array object
unsigned int bunnyVertexArrayObjID;
Model *m;

GLfloat a = 0;

// Reference to shader program
GLuint program;

GLuint myTex;


void OnTimer(int value)
{
	glutPostRedisplay();
	glutTimerFunc(20, &OnTimer, value);
}


void init(void)
{
	// vertex buffer object, used for uploading the geometry
  	 unsigned int bunnyVertexBufferObjID;
   	 unsigned int bunnyIndexBufferObjID;
   	 unsigned int bunnyNormalBufferObjID;
	 unsigned int bunnyTexCoordBufferObjID;

   	 m = LoadModel("bunnyplus.obj");

	dumpInfo();

	// Assignment: Change the color of the background.
	glClearColor(0.3,0.3,0.5,0);

	// Turn on Z-buffer
	glEnable(GL_DEPTH_TEST);
	//glDisable(GL_DEPTH_TEST);

	printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab2-4.vert", "lab2-4.frag");
	printError("init shader");

	// Upload geometry to the GPU:

	glGenVertexArrays(1, &bunnyVertexArrayObjID);
	glGenBuffers(1, &bunnyVertexBufferObjID);
	glGenBuffers(1, &bunnyIndexBufferObjID);
	glGenBuffers(1, &bunnyNormalBufferObjID);
	glGenBuffers(1, &bunnyTexCoordBufferObjID);
	glBindVertexArray(bunnyVertexArrayObjID);


	// VBO for vertex data
    glBindBuffer(GL_ARRAY_BUFFER, bunnyVertexBufferObjID);
    glBufferData(GL_ARRAY_BUFFER, m->numVertices*3*sizeof(GLfloat), m->vertexArray, GL_STATIC_DRAW);
    glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));
    printError("init vertices");

    // VBO for normal data
    glBindBuffer(GL_ARRAY_BUFFER, bunnyNormalBufferObjID);
    glBufferData(GL_ARRAY_BUFFER, m->numVertices*3*sizeof(GLfloat), m->normalArray, GL_STATIC_DRAW);
    glVertexAttribPointer(glGetAttribLocation(program, "in_Normal"), 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(glGetAttribLocation(program, "in_Normal"));
    printError("init normals");

    // VBO for index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bunnyIndexBufferObjID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m->numIndices*sizeof(GLuint), m->indexArray, GL_STATIC_DRAW);
    printError("init index");

	// VBO for texture coordinates
	if (m->texCoordArray != NULL)
	{
		glBindBuffer(GL_ARRAY_BUFFER, bunnyTexCoordBufferObjID);
		glBufferData(GL_ARRAY_BUFFER, m->numVertices*2*sizeof(GLfloat), m->texCoordArray, GL_STATIC_DRAW);
		glVertexAttribPointer(glGetAttribLocation(program, "in_Texture"), 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(glGetAttribLocation(program, "in_Texture"));

	}
	printError("init texture coordinates");

	glBindTexture(GL_TEXTURE_2D, myTex);
	glUniform1i(glGetUniformLocation(program, "texUnit"), 0); // Texture unit 0
	LoadTGATextureSimple("maskros512.tga", &myTex);

	glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);


	// End of upload of geometry
}


void display(void)
{
	printError("pre display");

	// Clear the screen and Z-buffer
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

	mat4 rot, trans, total, worldToView;

	a += 0.01;

	/*
	 * void gluLookAt(GLdouble eyeX,  GLdouble eyeY,  GLdouble eyeZ,
	 * 				  GLdouble centerX,  GLdouble centerY,  GLdouble centerZ,
	 * 				  GLdouble upX,  GLdouble upY,  GLdouble upZ);
	 */
	worldToView = lookAt(0,0,6, 0,0,0, 0,1,0);

	rot = Ry(a);
	trans = T(1, 0, -2);
	total = Mult(rot, trans);

	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, worldToView.m);



	glBindVertexArray(bunnyVertexArrayObjID);
    glDrawElements(GL_TRIANGLES, m->numIndices, GL_UNSIGNED_INT, 0L);

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
	glutCreateWindow ("lab2-1");
	glutDisplayFunc(display);
	init ();

    // lab1-3
    glutTimerFunc(20, &OnTimer, 0);

	glutMainLoop();
	return 0;
}

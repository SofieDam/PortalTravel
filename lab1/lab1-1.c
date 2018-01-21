// Lab 1-1.
// This is the same as the first simple example in the course book,
// but with a few error checks.
// Remember to copy your file to a new on appropriate places during the lab so you keep old results.
// Note that the files "lab1-1.frag", "lab1-1.vert" are required.

// Should work as is on Linux and Mac. MS Windows needs GLEW or glee.
// See separate Visual Studio version of my demos.
#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	// Linking hint for Lightweight IDE
	// uses framework Cocoa
#endif
#include "MicroGlut.h"
#include "GL_utilities.h"

// Globals
// Data would normally be read from files
/*
GLfloat vertices[] =
{
	-0.5f,-0.5f,0.0f,	// bottom left corner
	-0.5f,0.5f,0.0f,	// top corner
	0.5f,-0.5f,0.0f		// bottom right corner
};
*/ 

// Assignment: Try changing the triangle data, by moving the vertices.

GLfloat vertices[] =
{
	-0.8f,-0.8f,0.0f,
	-0.8f,0.8f,0.0f,
	0.8f,-0.8f,0.0f
};


// Assignment: Change the color of the triangle.
// Could have used an unit with only one vertex instead of in with 3 vertices,
// since I want all of the sides of the triangle to have the same color.
GLfloat triangleColor[] =
{
		1.0f, 0.5f, 1.0f,
		1.0f, 0.5f, 1.0f,
		1.0f, 0.5f, 1.0f
};




// vertex array object
unsigned int vertexArrayObjID;

void init(void)
{
	// vertex buffer object, used for uploading the geometry
	unsigned int vertexBufferObjID;

	// Assignment: Change the color of the triangle.
	unsigned int triangleColorBufferObjID;

	// Reference to shader program
	GLuint program;

	dumpInfo();

	// GL inits
	//glClearColor(0.2,0.2,0.5,0);

	// Assignment: Change the color of the background.
	glClearColor(0.5,0.3,0.5,0);

	glDisable(GL_DEPTH_TEST);
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab1-1.vert", "lab1-1.frag");
	printError("init shader");
	
	// Upload geometry to the GPU:
	
	// Allocate and activate Vertex Array Object
	glGenVertexArrays(1, &vertexArrayObjID);
	glBindVertexArray(vertexArrayObjID);
	// Allocate Vertex Buffer Objects
	glGenBuffers(1, &vertexBufferObjID);

	// Assignment: Change the color of the triangle.
	glGenBuffers(1, &triangleColorBufferObjID);


	// VBO for vertex data
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, 9*sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));

	// Assignment: Change the color of the triangle.
	// VBO for triangle color
	glBindBuffer(GL_ARRAY_BUFFER, triangleColorBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, 9*sizeof(GLfloat), triangleColor, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_Triangle_Color"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Triangle_Color"));

	// End of upload of geometry
	
	printError("init arrays");
}


void display(void)
{
	printError("pre display");

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT);

	glBindVertexArray(vertexArrayObjID);	// Select VAO
	glDrawArrays(GL_TRIANGLES, 0, 3);	// draw object

	printError("display");
	
	glutSwapBuffers();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitContextVersion(3, 2);
	glutInitWindowSize (500, 500);
	glutCreateWindow ("lab1-1");
	glutDisplayFunc(display); 
	init ();
	glutMainLoop();
	return 0;
}

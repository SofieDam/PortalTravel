// Lab 4, terrain generation

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

mat4 projectionMatrix, modelMatrix;

// LooakAt vertices
vec3 cam = {0, 5, 8};
vec3 lookAtPoint = {2, 0, 2};
vec3 upVector = {0, 1, 0};
float speed = 0.5;

// Light matrices and vertices
Point3D lightSourcesColorsArr[] = { {1.0f, 0.0f, 0.0f},   // Red light
									{0.0f, 1.0f, 0.0f},   // Green light
									{0.0f, 0.0f, 1.0f},   // Blue light
									{1.0f, 1.0f, 1.0f} }; // White light
GLfloat specularExponent[] = {300.0, 300.0, 600.0, 500.0};
GLint isDirectional[] = {0,0,1,1};
Point3D lightSourcesDirectionsPositions[] = { {10.0f, 5.0f, 0.0f},   // Red light, positional
											  {0.0f, 5.0f, 10.0f},   // Green light, positional
											  {-1.0f, 0.0f, 0.0f},   // Blue light along X
											  {0.0f, 0.0f, -1.0f} }; // White light along Z

// Model
float modelPosX = 0, modelPosY = 0, modelPosZ = 0; // location
float modelSpeed = 0.01;

/*
 * Calculate map height for a point.
 */
float height(Model* m, int width, float x, float z)
{
	// 1) Calculate what quad the point falls into.
	// v1*--*v2
	//   | /|
	//   |/ |
	// v3*--*v4

    // floor() finds the nearest integer less than or equal to the parameter
    int row = floor(x);
    int column = floor(z);

    // All are points on the Y-axis.
    float v1 = m->vertexArray[(row + column * width)*3 + 1];
    float v2 = m->vertexArray[((row+1) + column * width)*3 + 1];
    float v3 = m->vertexArray[(row + (column+1) * width)*3 + 1];
    float v4 = m->vertexArray[((row+1) + (column+1) * width)*3 + 1];

	// 2) The quad is built from two triangles. Figure out which one to use.
    float xDiff = x-(float)column;
    float zDiff = z-(float)row;
    float h;
    if ((xDiff + zDiff) < 1.0)     // triangle v1, v2, v3
    {
        // 3) Calculate the height value. Plane equation.
        h = v1 + xDiff*(v2 - v1) + zDiff*(v3 - v1);
        return h;
    }
    else                          // triangle v2, v3, v4
    {
        // 3) Calculate the height value. Plane equation.
        h = v4 + (1.0-xDiff)*(v2 - v4) + (1.0-zDiff)*(v3 - v4);
        return h;
    }
}

Model* GenerateTerrain(TextureData *tex)
{
	printf("tex->width * tex->height = %i * %i \n",tex->width,tex->height);

	int vertexCount = tex->width * tex->height;
	int triangleCount = (tex->width-1) * (tex->height-1) * 2;	// lab4-1: 18 triangles
	int x, z;

	printf("triangleCount = %i \n",triangleCount);

	GLfloat *vertexArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *normalArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *texCoordArray = malloc(sizeof(GLfloat) * 2 * vertexCount);
	GLuint *indexArray = malloc(sizeof(GLuint) * triangleCount*3);

	printf("sizeof(GLuint) = %lu \n",sizeof(GLuint));
	printf("texCoordArray = %lu \n",sizeof(texCoordArray));
	printf("indexArray = %lu \n",sizeof(indexArray));


	printf("bpp %d\n", tex->bpp);
	for (x = 0; x < tex->width; x++)
		for (z = 0; z < tex->height; z++)
		{
// Vertex array. You need to scale this properly
			vertexArray[(x + z * tex->width)*3 + 0] = x / 1.0;
			vertexArray[(x + z * tex->width)*3 + 1] = tex->imageData[(x + z * tex->width) * (tex->bpp/8)] / 100.0;
			vertexArray[(x + z * tex->width)*3 + 2] = z / 1.0;


// Normal vectors. You need to calculate these.
// Pick three neighbor vertices, on all sides of the vertex, and use these three for calculating a normal vector.

			// Neighboring vertices.
			vec3 nv1, nv2, nv3;

			// nv1 is the current vertex
			nv1.x = vertexArray[(x + z * tex->width)*3 + 0];
			nv1.y = vertexArray[(x + z * tex->width)*3 + 1];
			nv1.z = vertexArray[(x + z * tex->width)*3 + 2];

			// If the neighboring vertex is an edge, set normal along y-axis.
			if((x-1 > 0) && (x-1 < tex->width))
			{
				nv2.x = vertexArray[((x-1) + z * tex->width)*3 + 0];
				nv2.y = vertexArray[((x-1) + z * tex->width)*3 + 1];
				nv2.z = vertexArray[((x-1) + z * tex->width)*3 + 2];
			}
			else
			{
				nv2.x = 0.0;
				nv2.y = 1.0;
				nv2.z = 0.0;
			}
			if((z-1 > 0) && (z-1 < tex->height))
			{
				nv3.x = vertexArray[(x + (z-1) * tex->width)*3 + 0];
				nv3.y = vertexArray[(x + (z-1) * tex->width)*3 + 1];
				nv3.z = vertexArray[(x + (z-1) * tex->width)*3 + 2];
			}
			else
			{
				nv3.x = 0.0;
				nv3.y = 1.0;
				nv3.z = 0.0;
			}

			// Calculate normal vector
			vec3 v1 = VectorSub(nv2, nv1);
			vec3 v2 = VectorSub(nv3, nv1);
			vec3 normalVector =  Normalize(CrossProduct(v1, v2));

			normalArray[(x + z * tex->width)*3 + 0] = normalVector.x;
			normalArray[(x + z * tex->width)*3 + 1] = normalVector.y;
			normalArray[(x + z * tex->width)*3 + 2] = normalVector.z;


// Texture coordinates. You may want to scale them.
			texCoordArray[(x + z * tex->width)*2 + 0] = x; // (float)x / tex->width;
			texCoordArray[(x + z * tex->width)*2 + 1] = z; // (float)z / tex->height;
		}

	for (x = 0; x < tex->width-1; x++)
		for (z = 0; z < tex->height-1; z++)
		{
		// Triangle 1
			indexArray[(x + z * (tex->width-1))*6 + 0] = x + z * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 1] = x + (z+1) * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 2] = x+1 + z * tex->width;
		// Triangle 2
			indexArray[(x + z * (tex->width-1))*6 + 3] = x+1 + z * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 4] = x + (z+1) * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 5] = x+1 + (z+1) * tex->width;
		}
	
	// End of terrain generation
	
	// Create Model and upload to GPU:

	Model* model = LoadDataToModel(
			vertexArray,
			normalArray,
			texCoordArray,
			NULL,
			indexArray,
			vertexCount,
			triangleCount*3);

	return model;
}


// vertex array object
Model *m, *m2, *tm;
// Reference to shader program
GLuint program;
GLuint tex1, tex2;
TextureData ttex; // terrain


void init(void)
{
	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	printError("GL inits");

	projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 500.0); // lab4-3: changed the last value

	// Load and compile shader
	program = loadShaders("terrain-4-3.vert", "terrain-4-3.frag");
	glUseProgram(program);
	printError("init shader");
	
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
	glUniform1i(glGetUniformLocation(program, "tex"), 0); // Texture unit 0
	LoadTGATextureSimple("maskros512.tga", &tex1);
    LoadTGATextureSimple("conc.tga", &tex2);
	
	// Load terrain data
	//LoadTGATextureData("fft-terrain.tga", &ttex);
	LoadTGATextureData("44-terrain.tga", &ttex);
	tm = GenerateTerrain(&ttex);
	printError("init terrain");

    // m = LoadModelPlus("groundsphere.obj");
    m = LoadModelPlus("octagon.obj");

	// Upload external light sources to shader
	glUniform3fv(glGetUniformLocation(program, "lightSourcesDirPosArr"), 4, &lightSourcesDirectionsPositions[0].x);
	glUniform3fv(glGetUniformLocation(program, "lightSourcesColorArr"), 4, &lightSourcesColorsArr[0].x);
	glUniform1fv(glGetUniformLocation(program, "specularExponent"), 4, specularExponent);
	glUniform1iv(glGetUniformLocation(program, "isDirectional"), 4, isDirectional);
}

void display(void)
{
	// Handle keyboard and update camera matrix
	keyboard(&cam, &lookAtPoint, &upVector, speed);

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    // Terrain

	mat4 total, modelView, camMatrix;

	printError("pre display");
	
	glUseProgram(program);

	camMatrix = lookAt(cam.x, cam.y, cam.z,
					   lookAtPoint.x, lookAtPoint.y, lookAtPoint.z,
					   upVector.x, upVector.y, upVector.z);

	modelView = IdentityMatrix();
	total = Mult(camMatrix, modelView);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	glUniform3f(glGetUniformLocation(program, "eyePosition"), cam.x, cam.y, cam.z);

	glBindTexture(GL_TEXTURE_2D, tex1);		// Bind Our Texture tex1

	DrawModel(tm, program, "inPosition", "inNormal", "inTexCoord");



	// Model

    modelPosX += modelSpeed;
    modelPosZ += modelSpeed;
    modelPosY = height(tm, ttex.width, modelPosX, modelPosZ);

    modelMatrix = Mult(total, T(modelPosX, modelPosY, modelPosZ));

    glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, modelMatrix.m);
    glUniform3f(glGetUniformLocation(program, "eyePosition"), cam.x, cam.y, cam.z);

    glBindTexture(GL_TEXTURE_2D, tex2);

    DrawModel(m, program, "inPosition", "inNormal", "inTexCoord");



	printError("display 2");
	
	glutSwapBuffers();
}

void timer(int i)
{
	glutTimerFunc(20, &timer, i);
	glutPostRedisplay();
}

void mouse(int x, int y)
{
	printf("%d %d\n", x, y);
}


int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitContextVersion(3, 2);
	glutInitWindowSize (600, 600);
	glutCreateWindow ("TSBK07 Lab 4");
	glutDisplayFunc(display);
	init ();
	glutTimerFunc(20, &timer, 0);

	//glutPassiveMotionFunc(mouse);
	glutKeyboardFunc(keyPressed);

	glutMainLoop();
	exit(0);
}

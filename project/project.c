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
//#include "keyboard.c"

mat4 projectionMatrix;

// vertex array object
Model *m;

// Reference to shader program
GLuint program;

GLuint tex1;
TextureData ttex; // terrain


// LooakAt vertices
vec3 cam = {0, 5, 8};
vec3 lookAtPoint = {2, 0, 2};
vec3 upVector = {0, 1, 0};
//float speed = 0.5;

void init(void)
{
    // GL inits
    glClearColor(0.2,0.2,0.5,0);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    printError("GL inits");

    projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 500.0);

    // Load and compile shader
    //program = loadShaders("terrain-4-3.vert", "terrain-4-3.frag");
    //printError("init shader");

    //glUniform1i(glGetUniformLocation(program, "tex"), 0); // Texture unit 0
    //LoadTGATextureSimple("maskros512.tga", &tex1);

    // Load terrain data
    //LoadTGATextureData("fft-terrain.tga", &ttex);
    //printError("init terrain");

}

void display(void)
{
    // Handle keyboard and update camera matrix
    //keyboard(&cam, &lookAtPoint, &upVector, speed);

    // clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



    // Terrain

    mat4 total, modelView, camMatrix, modelMatrix;

    printError("pre display");

    glUseProgram(program);
    glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);

    camMatrix = lookAt(cam.x, cam.y, cam.z,
                       lookAtPoint.x, lookAtPoint.y, lookAtPoint.z,
                       upVector.x, upVector.y, upVector.z);
    glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, camMatrix.m);


    //modelView = IdentityMatrix();
    //total = Mult(camMatrix, modelView);
    //glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
    //glUniform3f(glGetUniformLocation(program, "eyePosition"), cam.x, cam.y, cam.z);

   // glBindTexture(GL_TEXTURE_2D, tex1);		// Bind Our Texture tex1

    //DrawModel(tm, program, "inPosition", "inNormal", "inTexCoord");

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
    glutCreateWindow ("TSBK07 project");
    glutDisplayFunc(display);
    init ();
    glutTimerFunc(20, &timer, 0);

    //glutPassiveMotionFunc(mouse);
    //glutKeyboardFunc(keyPressed);

    glutMainLoop();
    exit(0);
}

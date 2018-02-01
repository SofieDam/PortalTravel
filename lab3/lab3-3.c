/*
 * Use -> for pointers; use . for objects.
 */
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


float height=0, angle=0, zoom=8;

GLfloat a = 0;
GLfloat bladeAngle = 0;



// Reference to shader program
GLuint program;

GLuint myTex;

// Page 187 in the coursebook
struct GraphicsEntity
{
    char *name;
    Model *m;
    mat4 translation;
    mat4 rotation;
    mat4 scale;

    struct GraphicsEntity *child;   // children of this node
    struct GraphicsEntity *next;    // children of the same parent as the current node

};

struct GraphicsEntity Windmill, Skybox, Ground;


void keyboard(unsigned char c, int x, int y)
{
    switch (c)
    {
        case 27:
            exit(0);
            printf("case 27\n");
            break;
        case GLUT_KEY_UP:
            height += 0.1;
            glutPostRedisplay();
            break;
        case GLUT_KEY_DOWN:
            height -= 0.1;
            glutPostRedisplay();
            break;
        case GLUT_KEY_RIGHT:
            angle += 0.1;
            glutPostRedisplay();
            break;
        case GLUT_KEY_LEFT:
            angle -= 0.1;
            glutPostRedisplay();
            break;
        case 'z':
            zoom -= 0.5;
            glutPostRedisplay();
            break;
        case 'x':
            zoom += 0.5;
            glutPostRedisplay();
            break;
    }
}

void OnTimer(int value)
{
	glutPostRedisplay();
	glutTimerFunc(20, &OnTimer, value);
}


void ground() {
    Ground.name = strdup("ground");
    Ground.m = LoadModelPlus("ground.obj");
    Ground.translation = T(0, 0, 0);
    Ground.rotation = Ry(0);
    Ground.scale = S(1.0, 1.0, 1.0);

    Skybox.child = NULL;
    Skybox.next = NULL;

    printError("init ground");
}

void skybox() {
    Skybox.name = strdup("skybox");
    Skybox.m = LoadModelPlus("skybox.obj");
    Skybox.translation = T(0, 0, 0);
    Skybox.rotation = Ry(0);
    Skybox.scale = S(1.0, 1.0, 1.0);

    Skybox.child = NULL;
    Skybox.next = NULL;

    printError("init skybox");
}


struct GraphicsEntity *windmillBlades(mat4 t, mat4 r, mat4 s, int bladeNr){
    struct GraphicsEntity *blade = malloc(sizeof(struct GraphicsEntity));
    blade->name = strdup("blade");
    blade->m = LoadModelPlus("windmill/blade.obj");
    blade->translation = Mult(t,T(0,1.8,0.95));
    blade->rotation = r;
    blade->scale = Mult(s,S(0.8,0.8,0.8));

    blade->child = NULL;

    if(bladeNr != 3){
        bladeNr++;
        mat4 bladeRotation = Mult(r,Rx(asin(1)));
        blade->next = windmillBlades(t, bladeRotation, s, bladeNr);
    }
    else {
        blade->next = NULL;
    }

    return blade;
}

struct GraphicsEntity *windmillBalcony(mat4 t, mat4 r, mat4 s){
    struct GraphicsEntity *roof = malloc(sizeof(struct GraphicsEntity));
    roof->name = strdup("balcony");
    roof->m = LoadModelPlus("windmill/windmill-balcony.obj");
    roof->translation = t;
    roof->rotation = r;
    roof->scale = s;

    roof->child = NULL;
    roof->next = windmillBlades(t, r, s, 0);

    return roof;
}

struct GraphicsEntity *windmillRoof(mat4 t, mat4 r, mat4 s){
    struct GraphicsEntity *roof = malloc(sizeof(struct GraphicsEntity));
    roof->name = strdup("roof");
    roof->m = LoadModelPlus("windmill/windmill-roof.obj");
    roof->translation = t;
    roof->rotation = r;
    roof->scale = s;

    roof->child = NULL;
    roof->next = windmillBalcony(t, r, s);

    return roof;
}

void windmill(){

    Windmill.name = strdup("walls");
    Windmill.m = LoadModelPlus("windmill/windmill-walls.obj");
    Windmill.translation = T(0, -1.5, 0);
    Windmill.rotation = Ry(asin(-1));
    //Windmill.rotation = Ry(0);
    Windmill.scale = S(0.2, 0.2, 0.2);

    Windmill.child = windmillRoof(Windmill.translation, Windmill.rotation, Windmill.scale);
    Windmill.next = NULL;

    printError("init windmill");
}





void init(void)
{
	dumpInfo();

	// Assignment: Change the color of the background.
	//glClearColor(0.4,0.6,0.8,0);
    glClearColor(0.0,0.0,0.0,0);

	// Turn on Z-buffer
	//glEnable(GL_DEPTH_TEST);
	glDisable(GL_DEPTH_TEST);
    printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab3-3.vert", "lab3-3.frag");
	printError("init shader");

    //windmill();
    skybox();

    glBindTexture(GL_TEXTURE_2D, myTex);	// makes a texture the current one
    glUniform1i(glGetUniformLocation(program, "texUnit"), 0); // Texture unit 0
    LoadTGATextureSimple("SkyBox512.tga", &myTex);

}

void draw(struct GraphicsEntity entity)
{

    a += 0.005;
    bladeAngle += 0.008;


    /*
 * void gluLookAt(GLdouble eyeX,  GLdouble eyeY,  GLdouble eyeZ,
 * 				  GLdouble centerX,  GLdouble centerY,  GLdouble centerZ,
 * 				  GLdouble upX,  GLdouble upY,  GLdouble upZ);
 */
    mat4 worldToView = lookAt(zoom*sin(angle),zoom*height,zoom*cos(angle), 0,0,0, 0,1,0);
    //mat4 worldToView = lookAt(8*cos(a),0,8*sin(a), 0,0,0, 0,1,0);

    // Add rotation to the blades
    if(!strcmp(entity.name,"blade")) {
        mat4 bladeRotation = Mult(entity.rotation, Rx(bladeAngle));
        entity.rotation = bladeRotation;
    }

    mat4 total = Mult(Mult(entity.translation, entity.rotation), entity.scale);

    glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
    glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, worldToView.m);
    glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);

    DrawModel(entity.m, program, "in_Position", "in_Normal", "in_Tex_Coord");

    if(entity.child != NULL){
        draw(*entity.child);
    }
    if(entity.next != NULL){
        draw(*entity.next);
    }

}

void display(void)
{
	printError("pre display");

	// Clear the screen and Z-buffer
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

    //draw(Windmill);
    draw(Skybox);

	glutSwapBuffers();
}


int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitContextVersion(3, 2);

	// Set up Z-buffer
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutKeyboardFunc(keyboard);

    glutInitWindowSize (500, 500);
    glutCreateWindow ("lab3-3");
    glutDisplayFunc(display);
    init ();

    glutTimerFunc(20, &OnTimer, 0);

    glutMainLoop();
    return 0;
}

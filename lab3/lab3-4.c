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
#include "keyboard.c"


#define near 1.0
#define far 30.0
#define right 0.5
#define left -0.5
#define top 0.5
#define bottom -0.5


// Reference to shader program
GLuint program, lightProgram, sTex, gTex, cTex;


// Modified projection matrix, see p.59 in course book
GLfloat projectionMatrix[] = {    2.0f*near/(right-left), 0.0f, (right+left)/(right-left), 0.0f,
								  0.0f, 2.0f*near/(top-bottom), (top+bottom)/(top-bottom), 0.0f,
								  0.0f, 0.0f, -(far + near)/(far - near), -2*far*near/(far - near),
								  0.0f, 0.0f, -1.0f, 0.0f };




// lab3-4
Point3D lightSourcesColorsArr[] = { {1.0f, 0.0f, 0.0f},   // Red light
                                    {0.0f, 1.0f, 0.0f},   // Green light
                                    {0.0f, 0.0f, 1.0f},   // Blue light
                                    {1.0f, 1.0f, 1.0f} }; // White light
GLfloat specularExponent[] = {10.0, 20.0, 60.0, 5.0};
GLint isDirectional[] = {0,0,1,1};
Point3D lightSourcesDirectionsPositions[] = { {10.0f, 5.0f, 0.0f},   // Red light, positional
                                              {0.0f, 5.0f, 10.0f},   // Green light, positional
                                              {-1.0f, 0.0f, 0.0f},   // Blue light along X
                                              {0.0f, 0.0f, -1.0f} }; // White light along Z




float height=0, angle=0, zoom=8;
GLfloat a = 0;
GLfloat bladeAngle = 0;




// Page 187 in the coursebook
struct GraphicsEntity
{
    char *name;
    Model *m;
    mat4 translation;
    mat4 rotation;
    mat4 scale;
    GLuint texture;
    GLuint program;

    struct GraphicsEntity *child;   // children of this node
    struct GraphicsEntity *next;    // children of the same parent as the current node

};

struct GraphicsEntity Windmill, Skybox, Ground, Teapot;


void OnTimer(int value)
{
	glutPostRedisplay();
	glutTimerFunc(20, &OnTimer, value);
}


void teapot() {
    Teapot.name = strdup("teapot");
    Teapot.m = LoadModelPlus("teapot.obj");
    Teapot.translation = T(0, -1.5, 0);
    Teapot.rotation = Ry(0);
    Teapot.scale = S(0.3, 0.3, 0.3);
    Teapot.texture = cTex;
    Teapot.program = lightProgram;

    Teapot.child = NULL;
    Teapot.next = NULL;

    printError("init teapot");
}

void ground() {
    Ground.name = strdup("ground");
    Ground.m = LoadModelPlus("cubeplus.obj");
    Ground.translation = T(0, -1.5, 0);
    Ground.rotation = Ry(0);
    Ground.scale = S(50, 0.01, 50);
    Ground.texture = gTex;
    Ground.program = program;

    Ground.child = NULL;
    Ground.next = NULL;

    printError("init ground");
}

void skybox() {
    Skybox.name = strdup("skybox");
    Skybox.m = LoadModelPlus("skybox.obj");
    Skybox.translation = T(0, 0, 0);
    Skybox.rotation = Ry(0);
    Skybox.scale = S(1.0, 1.0, 1.0);
    Skybox.texture = sTex;
    Skybox.program = program;

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
    blade->texture = cTex;
    blade->program = lightProgram;

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
    struct GraphicsEntity *balcony = malloc(sizeof(struct GraphicsEntity));
    balcony->name = strdup("balcony");
    balcony->m = LoadModelPlus("windmill/windmill-balcony.obj");
    balcony->translation = t;
    balcony->rotation = r;
    balcony->scale = s;
    balcony->texture = cTex;
    balcony->program = lightProgram;


    balcony->child = NULL;
    balcony->next = windmillBlades(t, r, s, 0);

    return balcony;
}

struct GraphicsEntity *windmillRoof(mat4 t, mat4 r, mat4 s){
    struct GraphicsEntity *roof = malloc(sizeof(struct GraphicsEntity));
    roof->name = strdup("roof");
    roof->m = LoadModelPlus("windmill/windmill-roof.obj");
    roof->translation = t;
    roof->rotation = r;
    roof->scale = s;
    roof->texture = cTex;
    roof->program = lightProgram;


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
    Windmill.texture = cTex;
    Windmill.program = lightProgram;

    Windmill.child = windmillRoof(Windmill.translation, Windmill.rotation, Windmill.scale);
    Windmill.next = NULL;

    printError("init windmill");
}


void draw(struct GraphicsEntity entity)
{

    a += 0.005;
    bladeAngle += 0.008;

    glUseProgram(entity.program);

    // lab4-3
    // Upload external light sources to shader
    glUniform3fv(glGetUniformLocation(entity.program, "lightSourcesDirPosArr"), 4, &lightSourcesDirectionsPositions[0].x);
    glUniform3fv(glGetUniformLocation(entity.program, "lightSourcesColorArr"), 4, &lightSourcesColorsArr[0].x);
    glUniform1fv(glGetUniformLocation(entity.program, "specularExponent"), 4, specularExponent);
    glUniform1iv(glGetUniformLocation(entity.program, "isDirectional"), 4, isDirectional);


    //vec3 eyeCoordinate = zoom*sin(angle),zoom*height,zoom*cos(angle);
    mat4 worldToView = lookAt(zoom*sin(angle),zoom*height,zoom*cos(angle), 0,0,0, 0,1,0);
    glUniform3f(glGetUniformLocation(entity.program, "eyePosition"), zoom*sin(angle), zoom*height, zoom*cos(angle));

    // center the skybox around the camera (on origin in view coordinates)
    // zero out the translation part
    // see page 142 in the course book
    if (!strcmp(entity.name,"skybox") || !strcmp(entity.name,"ground")) {
        worldToView.m[3] = 0;
        worldToView.m[7] = 0;
        worldToView.m[11] = 0;
    }

    // Add rotation to the blades
    if(!strcmp(entity.name,"blade")) {
        mat4 bladeRotation = Mult(entity.rotation, Rx(bladeAngle));
        entity.rotation = bladeRotation;
    }

    mat4 total = Mult(Mult(entity.translation, entity.rotation), entity.scale);

    glUniformMatrix4fv(glGetUniformLocation(entity.program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
    glUniformMatrix4fv(glGetUniformLocation(entity.program, "camMatrix"), 1, GL_TRUE, worldToView.m);
    glUniformMatrix4fv(glGetUniformLocation(entity.program, "mdlMatrix"), 1, GL_TRUE, total.m);

    glBindTexture(GL_TEXTURE_2D, entity.texture);	// makes a texture the current one
    glUniform1i(glGetUniformLocation(entity.program, "texUnit"), 0); // Texture unit 0

    if (!strcmp(entity.name,"skybox") || !strcmp(entity.name,"ground")) {
        DrawModel(entity.m, entity.program, "in_Position", NULL, "in_Tex_Coord");
    }
    else if (!strcmp(entity.name,"teapot")) {
        DrawModel(entity.m, entity.program, "in_Position", "in_Normal", NULL);
    }
    else {
        DrawModel(entity.m, entity.program, "in_Position", "in_Normal", "in_Tex_Coord");
    }

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

    // Handle keyboard inputs
    keyboard(&height, &angle, &zoom);

	// Clear the screen and Z-buffer
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

    // ------------------- Skybox -------------------
    // Turn off Z-buffer
    glDisable(GL_DEPTH_TEST);

    draw(Skybox);
    draw(Ground);
    // Turn on Z-buffer
    glEnable(GL_DEPTH_TEST);
    // --------------------------------------------

    //draw(Windmill);
    draw(Teapot);

    glutSwapBuffers();
}

void init(void)
{
    dumpInfo();

    // Assignment: Change the color of the background.
    //glClearColor(0.4,0.6,0.8,0);
    glClearColor(0.0,0.0,0.0,0);

    printError("GL inits");

    // Load and compile shader
    program = loadShaders("lab3-4.vert", "lab3-4.frag");
    lightProgram = loadShaders("lab3-4-light.vert", "lab3-4-light.frag");
    printError("init shader");

    // Load textures
    LoadTGATextureSimple("grass.tga", &gTex);
    LoadTGATextureSimple("SkyBox512.tga", &sTex);
    LoadTGATextureSimple("conc.tga", &cTex);
    printError("load textures");

    // Load objects
    ground();
    //windmill();
    teapot();
    skybox();

}


int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitContextVersion(3, 2);

	// Set up Z-buffer
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutInitWindowSize (500, 500);
    glutCreateWindow ("lab3-4");
    glutDisplayFunc(display);
    glutKeyboardFunc(keyPressed);
    init ();

    glutTimerFunc(20, &OnTimer, 0);

    glutMainLoop();
    return 0;
}

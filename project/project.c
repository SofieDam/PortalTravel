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

mat4 projectionMatrix;

// vertex array object
Model *m, *t;

// Reference to shader program
GLuint program;

GLuint tex;
TextureData ttex; // terrain


// LooakAt vertices
float height=0, angle=0, zoom=1;
/*
vec3 cam = {0, 1, 1};
vec3 lookAtPoint = {0, 0, 0};
vec3 upVector = {0, 1, 0};
float speed = 0.1;
 */



// xyz-plane for each side of the cube
/*
static const vec3 xyz_plane[4] = {
        {1.0, 1.0, 0.0},              // front
        {0.0, 1.0, -1.0},              // right
        {1.0, 1.0,  -1.0},             // back
        {-1.0, 1.0, -1.0}              // left
};
 */



/*
GLfloat cube_vertices[] = {
        // front
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,

        // right
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, 1.0f,

        // back
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,

        // left
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,

        // upper
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,

        // bottom
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f
        };

GLuint cube_indices[] = {
        0,  1,  2,  0,  2,  3,   //front
        4,  5,  6,  4,  6,  7,   //right
        8,  9,  10, 8,  10, 11,  //back
        12, 13, 14, 12, 14, 15,  //left
        16, 17, 18, 16, 18, 19,  //upper
        20, 21, 22, 20, 22, 23}; //bottom
*/



// First vector (lower left) to generate a cube side
static vec3 cube_vertices[6] = {
        {-1.0, -1.0, 1.0},            // front
        {1.0, -1.0, 1.0},             // right
        {1.0, -1.0, -1.0},            // back
        {-1.0, -1.0, -1.0},           // left
        {-1.0, 1.0, 1.0},            // up
        {1.0, -1.0, 1.0},            // bottom
};

static mat3 cube_xyz_plane[6] = {
        {1.0, 0.0, 0.0,   0.0, 1.0, 0.0,   0.0, 0.0, 0.0},     // front
        {0.0, 0.0, 0.0,   0.0, 1.0, 0.0,  -1.0, 0.0, 0.0},     //right
        {-1.0, 0.0, 0.0,   0.0, 1.0, 0.0,   0.0, 0.0, 0.0},    // back
        {0.0, 0.0, 0.0,   0.0, 1.0, 0.0,   1.0, 0.0, 0.0},     // left
        {1.0, 0.0, 0.0,   0.0, 0.0, 0.0,   0.0, -1.0, 0.0},    // up
        {-1.0, 0.0, 0.0,   0.0, 0.0, 0.0,   0.0, -1.0, 0.0},   // bottom
};

Model* generateSphere()
{
#define PI 3.14159265358979323846f

/*
    int vertexCount = 4*6;
    int triangleCount = 4*6* 2;

    GLfloat *vertexArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
    //GLfloat *normalArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
    //GLfloat *texCoordArray = malloc(sizeof(GLfloat) * 2 * vertexCount);
    GLuint *indexArray = malloc(sizeof(GLuint) * triangleCount*3);

    vertexArray = cube_vertices;
    indexArray = cube_indices;
*/


    //int width = 2;
    //int height = 2;

    float radius = 1.0;
    vec3 radius_vec = SetVector(radius,radius,radius);
    //int w = ttex.width;
    int w = 2;

    float cube_width = 2.0;
    float square_width = cube_width / (float)w;
    printf("w = %f\n", (float)w);
    printf("cube_width = %f\n", cube_width);
    printf("square_width = %f\n", square_width);
    printf("\n");

    int cube_sides = 6;

    int vertexCount = w*w*cube_sides;
    int triangleCount = (w-1) * (w-1) * 2 * cube_sides;

    GLfloat *vertexArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
    //GLfloat *normalArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
    GLfloat *texCoordArray = malloc(sizeof(GLfloat) * 2 * vertexCount);
    GLuint *indexArray = malloc(sizeof(GLuint) * triangleCount*3);



    int i, r, c;
    vec3 r_c;
    vec3 sphere_r_c;
    for( i=0; i<cube_sides; i++ )
        for( c=0; c<(w); c++ ) {
            for( r=0; r<(w); r++ ) {

                printf("c = %i, r = %i \n",c,r);

                r_c = SetVector(r, c, 0);
                r_c = ScalarMult(r_c, w);
                r_c = MultMat3Vec3(cube_xyz_plane[i],r_c);
                r_c = VectorAdd(cube_vertices[i], r_c);


                // Transform to sphere positions
                //r_c = mix(r_c,radius_vec,1.0f);
                //r_c =
                //sphere_r_c = Normalize(r_c);
                //r_c = mix(r_c,sphere_r_c,1.0);

                vertexArray[(r + (c*w) + (i*w*w)) * 3 + 0] = r_c.x;
                vertexArray[(r + (c*w) + (i*w*w)) * 3 + 1] = r_c.y;
                vertexArray[(r + (c*w) + (i*w*w)) * 3 + 2] = r_c.z;

                printf("vertexArray[%i].x = %f \n",((r + (c*w) + (i*w*w)) * 3 + 0), r_c.x);
                printf("vertexArray[%i].y = %f \n",((r + (c*w) + (i*w*w)) * 3 + 1), r_c.y);
                printf("vertexArray[%i].z = %f \n", ((r + (c*w) + (i*w*w)) * 3 + 2), r_c.z);



                texCoordArray[(r + (c*w) + (i*w*w))*2 + 0] = -r; // (float)x / tex->width;
                texCoordArray[(r + (c*w) + (i*w*w))*2 + 1] = -c; // (float)z / tex->height;

                }
            }


    // Calculate indices
    for( i=0; i<cube_sides; i++ )
        for( c=0; c<(w-1); c++ )
            for( r=0; r<(w-1); r++ ) {
                //printf("\n");

                // Triangle 1

                //indexArray[(r + c * (w-1))*6 + 0] = vertexArray[(r + c * w) * 3 + 0];
                //indexArray[(r + c * (w-1))*6 + 1] = vertexArray[(r + (c+1) * w) * 3 + 0];
                //indexArray[(r + c * (w-1))*6 + 2] = vertexArray[(r+1 + c * w) * 3 + 0];

                indexArray[(r + (c*(w-1)) + (i*(w-1)*(w-1)))*6 + 0] = r + (c*w) + (i*w*w);
                indexArray[(r + (c*(w-1)) + (i*(w-1)*(w-1)))*6 + 1] = (r+1) + (c*w) + (i*w*w);
                indexArray[(r + (c*(w-1)) + (i*(w-1)*(w-1)))*6 + 2] = r + ((c+1)*w) + (i*w*w);

                // Triangle 2

                //indexArray[(r + c * (w-1))*6 + 0] = vertexArray[((r+1) + c * w) * 3 + 0];
                //indexArray[(r + c * (w-1))*6 + 1] = vertexArray[(r + (c+1) * w) * 3 + 0];
                //indexArray[(r + c * (w-1))*6 + 2] = vertexArray[((r+1) + (c+1) * w) * 3 + 0];

                indexArray[(r + (c*(w-1)) + (i*(w-1)*(w-1)))*6 + 3] = (r+1) + (c*w) + (i*w*w);
                indexArray[(r + (c*(w-1)) + (i*(w-1)*(w-1)))*6 + 4] = r + ((c+1)*w) + (i*w*w);
                indexArray[(r + (c*(w-1)) + (i*(w-1)*(w-1)))*6 + 5] = (r+1) + ((c+1)*w) + (i*w*w);

                /*
                printf("ia_%i: %i\n", ((r + c * (width-1))*6 + 0), (indexArray[(r + c * (width-1))*6 + 0]));
                printf("ia_%i: %i\n", ((r + c * (width-1))*6 + 1), (indexArray[(r + c * (width-1))*6 + 1]));
                printf("ia_%i: %i\n", ((r + c * (width-1))*6 + 2), (indexArray[(r + c * (width-1))*6 + 2]));
                printf("ia_%i: %i\n", ((r + c * (width-1))*6 + 3), (indexArray[(r + c * (width-1))*6 + 3]));
                printf("ia_%i: %i\n", ((r + c * (width-1))*6 + 4), (indexArray[(r + c * (width-1))*6 + 4]));
                printf("ia_%i: %i\n", ((r + c * (width-1))*6 + 5), (indexArray[(r + c * (width-1))*6 + 5]));
                printf("\n");
                 */
            }



    /*
    Model* model = LoadDataToModel(
            vertexArray,
            normalArray,
            texCoordArray,
            NULL,
            indexArray,
            vertexCount,
            triangleCount*3);
     */

// Create Model and upload to GPU
    Model* model = LoadDataToModel(
            vertexArray,
            NULL,
            texCoordArray,
            NULL,
            indexArray,
            vertexCount,
            triangleCount*3);

    //free(indexArray);
    //free(vertexArray);

    return model;
}


void init(void)
{
    // GL inits
    glClearColor(0.2,0.5,0.8,0);
    glEnable(GL_DEPTH_TEST);
    //glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    printError("GL inits");

    projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 500.0);

    // Load and compile shader
    program = loadShaders("terrain.vert", "terrain.frag");
    printError("init shader");

    glUniform1i(glGetUniformLocation(program, "tex"), 0); // Texture unit 0
    LoadTGATextureSimple("image/maskros512.tga", &tex);

    // Load terrain data
    LoadTGATextureData("image/fft-terrain.tga", &ttex);
    printError("init terrain");

    //m = LoadModelPlus("cubeplus.obj");

    //init_matrices();
    m = generateSphere();

    //t = LoadModelPlus("cubeplus.obj");


}

void display(void)
{
    // Handle keyboard and update camera matrix
    keyboard(&height, &angle, &zoom);
    //keyboard(&cam, &lookAtPoint, &upVector, speed);

    // clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Terrain

    mat4 camMatrix, identityMatrix, modelMatrix;

    printError("pre display");

    glUseProgram(program);
    glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);

    /*
    camMatrix = lookAt(cam.x, cam.y, cam.z,
                       lookAtPoint.x, lookAtPoint.y, lookAtPoint.z,
                       upVector.x, upVector.y, upVector.z);
                       */
    camMatrix = lookAt(zoom*sin(angle),zoom*height,zoom*cos(angle), 0,0,0, 0,1,0);
    glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, camMatrix.m);


    identityMatrix = IdentityMatrix();
    glUniformMatrix4fv(glGetUniformLocation(program, "identityMatrix"), 1, GL_TRUE, identityMatrix.m);

    //modelMatrix = Mult(S(0.5, 0.5, 0.5),T(0,0,-4));
    modelMatrix = S(0.3, 0.3, 0.3);
    glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_TRUE, modelMatrix.m);


    glBindTexture(GL_TEXTURE_2D, tex);		// Bind Our Texture tex

    //DrawModel(m, program, "inPosition", "inNormal", "inTexCoord");

    glUniform3f(glGetUniformLocation(program, "color"), 0.9, 0.7, 0.9);


    DrawModel(m, program, "inPosition", NULL, "inTexCoord");

    //DrawModel(t, program, "inPosition", NULL, NULL);

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
    glutInitWindowSize (700, 700);
    glutCreateWindow ("TSBK07 project");
    glutDisplayFunc(display);
    init ();
    glutTimerFunc(20, &timer, 0);

    //glutPassiveMotionFunc(mouse);
    glutKeyboardFunc(keyPressed);

    glutMainLoop();
    exit(0);
}

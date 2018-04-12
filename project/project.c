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
#include <time.h>

mat4 projectionMatrix;

// vertex array object
Model *sphere, *ocean;

// Reference to shader program
GLuint program, program_ocean;

GLuint tex;
TextureData ttex; // terrain

GLfloat *sphereVertexArray;

// LooakAt vertices
float height=0, angle=0, zoom=1.0;
/*
vec3 cam = {0, 1, 1};
vec3 lookAtPoint = {0, 0, 0};
vec3 upVector = {0, 1, 0};
float speed = 0.01;
*/

time_t rawtime;
struct tm * currentTime;

// First vector (lower left corner) to generate a cube side
static vec3 cube_vertices[6] = {
        {-1.0, -1.0, 1.0},            // front
        {1.0, -1.0, 1.0},             // right
        {1.0, -1.0, -1.0},            // back
        {-1.0, -1.0, -1.0},           // left
        {-1.0, 1.0, 1.0},            // up
        {1.0, -1.0, 1.0},            // bottom
};
// XYZ-transformation for each cube side
static mat3 cube_xyz_plane[6] = {
        // x    y    z      x    y    z      x    y    z
        {{1.0, 0.0, 0.0,   0.0, 1.0, 0.0,   0.0, 0.0, 1.0}},     // front
        {{0.0, 0.0, 1.0,   0.0, 1.0, 0.0,   -1.0, 0.0, 0.0}},     //right
        {{-1.0, 0.0, 0.0,   0.0, 1.0, 0.0,   0.0, 0.0, -1.0}},    // back
        {{0.0, 0.0, -1.0,   0.0, 1.0, 0.0,   1.0, 0.0, 0.0}},     // left
        {{1.0, 0.0, 0.0,   0.0, 0.0, 1.0,   0.0, -1.0, 0.0}},    // up
        {{-1.0, 0.0, 0.0,   0.0, 0.0, -1.0,   0.0, -1.0, 0.0}},   // bottom
};



vec3 calculateNormalVector(vec3 nv1, vec3 nv2, vec3 nv3){
    vec3 v1 = VectorSub(nv2, nv1);
    vec3 v2 = VectorSub(nv3, nv1);
    vec3 normalVector =  Normalize(CrossProduct(v1, v2));
    return normalVector;
}



Model* generateOcean()
{
    int i, r, c;

    int w = ttex.width+1;
    int cube_sides = 6;

    int vertexCount = sphere->numVertices;
    GLfloat *normalArray = malloc(sizeof(GLfloat) * 3 * vertexCount);


    // Calculate normal vectors.
    // Pick three neighbor vertices, on all sides of the vertex,
    // and use these three for calculating a normal vector.
    vec3 nv1, nv2, nv3;    // Neighboring vertices
    for( i=0; i<cube_sides; i++ ) {
        for( c=0; c<(w-1); c++ )
            for( r=0; r<(w-1); r++ ) {

                nv1.x = sphereVertexArray[(r + (c*w) + (i*w*w))*3 + 0];
                nv1.y = sphereVertexArray[(r + (c*w) + (i*w*w))*3 + 1];
                nv1.z = sphereVertexArray[(r + (c*w) + (i*w*w))*3 + 2];

                nv2.x = sphereVertexArray[((r+1) + (c*w) + (i*w*w))*3 + 0];
                nv2.y = sphereVertexArray[((r+1) + (c*w) + (i*w*w))*3 + 1];
                nv2.z = sphereVertexArray[((r+1) + (c*w) + (i*w*w))*3 + 2];

                nv3.x = sphereVertexArray[(r + ((c+1)*w) + (i*w*w))*3 + 0];
                nv3.y = sphereVertexArray[(r + ((c+1)*w) + (i*w*w))*3 + 1];
                nv3.z = sphereVertexArray[(r + ((c+1)*w) + (i*w*w))*3 + 2];

                vec3 normalVector =  calculateNormalVector(nv1, nv2, nv3);

                normalArray[(r + (c*w) + (i*w*w))*3 + 0] = normalVector.x;
                normalArray[(r + (c*w) + (i*w*w))*3 + 1] = normalVector.y;
                normalArray[(r + (c*w) + (i*w*w))*3 + 2] = normalVector.z;

            }


        // Calculate the last normal vector.

        nv1.x = sphereVertexArray[(r + (c*w) + (i*w*w))*3 + 0];
        nv1.y = sphereVertexArray[(r + (c*w) + (i*w*w))*3 + 1];
        nv1.z = sphereVertexArray[(r + (c*w) + (i*w*w))*3 + 2];

        nv2.x = sphereVertexArray[((r-1) + (c*w) + (i*w*w))*3 + 0];
        nv2.y = sphereVertexArray[((r-1) + (c*w) + (i*w*w))*3 + 1];
        nv2.z = sphereVertexArray[((r-1) + (c*w) + (i*w*w))*3 + 2];

        nv3.x = sphereVertexArray[(r + ((c-1)*w) + (i*w*w))*3 + 0];
        nv3.y = sphereVertexArray[(r + ((c-1)*w) + (i*w*w))*3 + 1];
        nv3.z = sphereVertexArray[(r + ((c-1)*w) + (i*w*w))*3 + 2];

        vec3 normalVector =  calculateNormalVector(nv1, nv2, nv3);

        normalArray[(r + (c*w) + (i*w*w))*3 + 0] = normalVector.x;
        normalArray[(r + (c*w) + (i*w*w))*3 + 1] = normalVector.y;
        normalArray[(r + (c*w) + (i*w*w))*3 + 2] = normalVector.z;
    }


    // Create Model and upload to GPU
    Model* model = LoadDataToModel(
            sphereVertexArray,
            normalArray,
            sphere->texCoordArray,
            NULL,
            sphere->indexArray,
            vertexCount,
            sphere->numIndices);


    //free(indexArray);
    //free(vertexArray);

    return model;
}




// Quadratic distance function
// Lower the height around the circle
float generateIslandMask(float island_size, float r, float c)
{
    island_size = island_size * 0.5;
    float distance_r = fabs(r - island_size);  // fabs() calculate the absolute value
    float distance_c = fabs(c - island_size);
    float distance = sqrt(distance_r*distance_r + distance_c*distance_c); // circular mask

    island_size = island_size - 10.0;
    float delta = distance / island_size;
    float grad = delta * delta;

    float result = fmax(0.0, (1.0 - grad));

    return result;
}

/*
 * 1. Build a cube.
 * 2. Normalize every vertex which will result in a sphere.
 * 3. Add height (from greyscale-image) to sphere.
 */
Model* generateSphere()
{
    int i, r, c;
    vec3 r_c_cube, r_c_sphere, r_c_ocean;
    float island_height;

    int w = ttex.width;
    //int w = 2;
    //int w = 32;

    int cube_sides = 6;
    float cube_width = 2.0;
    float square_width = cube_width / (float)w;

    w += 1;             // need one extra vertex to scale from image properly

    int vertexCount = w*w*cube_sides;
    int triangleCount = (w-1) * (w-1) * 2 * cube_sides * 3;

    GLfloat *vertexArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
    GLfloat *normalArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
    GLfloat *texCoordArray = malloc(sizeof(GLfloat) * 2 * vertexCount);
    GLuint *indexArray = malloc(sizeof(GLuint) * triangleCount);

    sphereVertexArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
    float height_ocean = 0.008;

    float height = 0;
    float height_textureData = 0;

    // Calculate vertices
    for( i=0; i<cube_sides; i++ )
        for( c=0; c<w; c++ ) {
            for( r=0; r<w; r++ ) {

                // Ground
                r_c_cube = SetVector(r, c, 0);
                r_c_cube = ScalarMult(r_c_cube, square_width);
                r_c_cube = MultMat3Vec3(cube_xyz_plane[i],r_c_cube);
                r_c_cube = VectorAdd(cube_vertices[i], r_c_cube);

                // Height
                height_textureData = (ttex.imageData[(r + (c*w)) * (ttex.bpp/8)] / 50.0);
                if (i==0) {
                    island_height = generateIslandMask((float)w, (float)r, (float)c);
                    height = height_textureData * square_width * island_height;
                }

                // Transform cube to sphere positions
                r_c_sphere = Normalize(r_c_cube);

                // Add ocean vertices
                //r_c_ocean = VectorAdd(r_c_sphere, ScalarMult(Normalize(r_c_sphere),(height_ocean + (height_textureData * square_width / 5)))); // heightmap on water surface
                r_c_ocean = VectorAdd(r_c_sphere, ScalarMult(Normalize(r_c_sphere),height_ocean));
                sphereVertexArray[(r + (c*w) + (i*w*w)) * 3 + 0] = r_c_ocean.x;
                sphereVertexArray[(r + (c*w) + (i*w*w)) * 3 + 1] = r_c_ocean.y;
                sphereVertexArray[(r + (c*w) + (i*w*w)) * 3 + 2] = r_c_ocean.z;

                // Add height
                r_c_sphere = VectorAdd(r_c_sphere, ScalarMult(Normalize(r_c_sphere),height));


                vertexArray[(r + (c*w) + (i*w*w)) * 3 + 0] = r_c_sphere.x;
                vertexArray[(r + (c*w) + (i*w*w)) * 3 + 1] = r_c_sphere.y;
                vertexArray[(r + (c*w) + (i*w*w)) * 3 + 2] = r_c_sphere.z;



                // Calculate texture coordinates
                texCoordArray[(r + (c*w) + (i*w*w))*2 + 0] = -r;
                texCoordArray[(r + (c*w) + (i*w*w))*2 + 1] = -c;
                }
            }


    vec3 nv1, nv2, nv3;    // Neighboring vertices
    for( i=0; i<cube_sides; i++ ) {
        for( c=0; c<(w-1); c++ )
            for( r=0; r<(w-1); r++ ) {

                // Calculate indices

                // Triangle 1
                indexArray[(r + (c*(w-1)) + (i*(w-1)*(w-1)))*6 + 0] = r + (c*w) + (i*w*w);
                indexArray[(r + (c*(w-1)) + (i*(w-1)*(w-1)))*6 + 1] = (r+1) + (c*w) + (i*w*w);
                indexArray[(r + (c*(w-1)) + (i*(w-1)*(w-1)))*6 + 2] = r + ((c+1)*w) + (i*w*w);
                // Triangle 2
                indexArray[(r + (c*(w-1)) + (i*(w-1)*(w-1)))*6 + 3] = (r+1) + (c*w) + (i*w*w);
                indexArray[(r + (c*(w-1)) + (i*(w-1)*(w-1)))*6 + 4] = r + ((c+1)*w) + (i*w*w);
                indexArray[(r + (c*(w-1)) + (i*(w-1)*(w-1)))*6 + 5] = (r+1) + ((c+1)*w) + (i*w*w);



                // Calculate normal vectors.
                // Pick three neighbor vertices, on all sides of the vertex,
                // and use these three for calculating a normal vector.

                nv1.x = vertexArray[(r + (c*w) + (i*w*w))*3 + 0];
                nv1.y = vertexArray[(r + (c*w) + (i*w*w))*3 + 1];
                nv1.z = vertexArray[(r + (c*w) + (i*w*w))*3 + 2];

                nv2.x = vertexArray[((r+1) + (c*w) + (i*w*w))*3 + 0];
                nv2.y = vertexArray[((r+1) + (c*w) + (i*w*w))*3 + 1];
                nv2.z = vertexArray[((r+1) + (c*w) + (i*w*w))*3 + 2];

                nv3.x = vertexArray[(r + ((c+1)*w) + (i*w*w))*3 + 0];
                nv3.y = vertexArray[(r + ((c+1)*w) + (i*w*w))*3 + 1];
                nv3.z = vertexArray[(r + ((c+1)*w) + (i*w*w))*3 + 2];

                vec3 normalVector =  calculateNormalVector(nv1, nv2, nv3);

                normalArray[(r + (c*w) + (i*w*w))*3 + 0] = normalVector.x;
                normalArray[(r + (c*w) + (i*w*w))*3 + 1] = normalVector.y;
                normalArray[(r + (c*w) + (i*w*w))*3 + 2] = normalVector.z;



            }


        // Calculate the last normal vector.

        nv1.x = vertexArray[(r + (c*w) + (i*w*w))*3 + 0];
        nv1.y = vertexArray[(r + (c*w) + (i*w*w))*3 + 1];
        nv1.z = vertexArray[(r + (c*w) + (i*w*w))*3 + 2];

        nv2.x = vertexArray[((r-1) + (c*w) + (i*w*w))*3 + 0];
        nv2.y = vertexArray[((r-1) + (c*w) + (i*w*w))*3 + 1];
        nv2.z = vertexArray[((r-1) + (c*w) + (i*w*w))*3 + 2];

        nv3.x = vertexArray[(r + ((c-1)*w) + (i*w*w))*3 + 0];
        nv3.y = vertexArray[(r + ((c-1)*w) + (i*w*w))*3 + 1];
        nv3.z = vertexArray[(r + ((c-1)*w) + (i*w*w))*3 + 2];

        vec3 normalVector =  calculateNormalVector(nv1, nv2, nv3);

        normalArray[(r + (c*w) + (i*w*w))*3 + 0] = normalVector.x;
        normalArray[(r + (c*w) + (i*w*w))*3 + 1] = normalVector.y;
        normalArray[(r + (c*w) + (i*w*w))*3 + 2] = normalVector.z;
    }



// Create Model and upload to GPU
    Model* model = LoadDataToModel(
            vertexArray,
            normalArray,
            texCoordArray,
            NULL,
            indexArray,
            vertexCount,
            triangleCount);

    //free(indexArray);
    //free(vertexArray);

    return model;
}


void init(void)
{
    // GL inits
    //glClearColor(0.2,0.5,0.8,0);
    glClearColor(0.6, 0.6, 0.9, 0);
    glEnable(GL_DEPTH_TEST);
    //glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    printError("GL inits");

    projectionMatrix = frustum(-0.001, 0.001, -0.001, 0.001, 0.002, 100.0);

    // Load and compile shader
    program = loadShaders("terrain.vert", "terrain.frag");
    printError("init shader");

    // Load texture
    glUniform1i(glGetUniformLocation(program, "tex"), 0); // Texture unit 0
    LoadTGATextureSimple("image/ground_sand_2.tga", &tex);
    printError("init load texture");

    // Load terrain data
    LoadTGATextureData("image/fft-terrain.tga", &ttex);
    printError("init terrain data");

    // Generate planet
    sphere = generateSphere();
    ocean = generateOcean();
    printError("init terrain model");

    program_ocean = loadShaders("ocean.vert", "ocean.frag");


}

void display(void)
{
    // Handle keyboard and update camera matrix
    keyboard(&height, &angle, &zoom);
    //keyboard(&cam, &lookAtPoint, &upVector, speed);

    // clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    mat4 camMatrix, identityMatrix, modelMatrix;

    printError("pre display");

    // ---------------------------      Ground       ---------------------------

    glUseProgram(program);
    glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);

    camMatrix = lookAt(zoom*sin(angle),zoom*height,zoom*cos(angle), 0,0,0, 0,1,0);
    /*
    camMatrix = lookAt(cam.x, cam.y, cam.z,
                       lookAtPoint.x, lookAtPoint.y, lookAtPoint.z,
                       upVector.x, upVector.y, upVector.z);
                       */
    glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, camMatrix.m);

    identityMatrix = IdentityMatrix();
    glUniformMatrix4fv(glGetUniformLocation(program, "identityMatrix"), 1, GL_TRUE, identityMatrix.m);

    modelMatrix = S(0.3, 0.3, 0.3);
    glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_TRUE, modelMatrix.m);


    glBindTexture(GL_TEXTURE_2D, tex);		// Bind Our Texture tex
    //glUniform3f(glGetUniformLocation(program, "color"), 0.9, 0.7, 0.9);

    DrawModel(sphere, program, "inPosition", "inNormal", "inTexCoord");




    // ---------------------------      Ocean       ---------------------------

    glUseProgram(program_ocean);

    glUniformMatrix4fv(glGetUniformLocation(program_ocean, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
    glUniformMatrix4fv(glGetUniformLocation(program_ocean, "camMatrix"), 1, GL_TRUE, camMatrix.m);
    glUniformMatrix4fv(glGetUniformLocation(program_ocean, "identityMatrix"), 1, GL_TRUE, identityMatrix.m);
    glUniformMatrix4fv(glGetUniformLocation(program_ocean, "modelMatrix"), 1, GL_TRUE, modelMatrix.m);

    // Enable blending for transparency
    // http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-10-transparency/
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Get current time
    time ( &rawtime );
    currentTime = localtime ( &rawtime );
    //printf ( "Current time: %d\n", timeinfo->tm_sec );
    glUniform1f(glGetUniformLocation(program_ocean, "currentTime"), (float)currentTime->tm_sec);

    DrawModel(ocean, program_ocean, "inPosition", "inNormal", NULL);

    // Disable blending
    glDisable(GL_BLEND);



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
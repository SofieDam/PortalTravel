#include <time.h>
#include <math.h>


// Model
Model *island, *ocean;

// Shader program
GLuint program_island, program_ocean;

// Texture
GLuint tex_sand;
TextureData ttex_island; // terrain

// Sphere vertices for ocean
GLfloat *sphereVertexArray;

// Position
mat4 projectionMatrix_island, identityMatrix_island, modelMatrix_island, camMatrix_island;

// Camera
float R_island, verticalAngle_island, horizontalAngle_island, horizontalHeadAngle_island;

// Time
time_t rawtime;
struct tm * currentTime;

// Height for ocean
const float height_ocean = 0.008;


Model* generateOcean()
{
    int i, r, c;

    int w = ttex_island.width+1;

    int vertexCount = island->numVertices;
    GLfloat *normalArray = malloc(sizeof(GLfloat) * 3 * vertexCount);


    for( i=0; i<cube_sides; i++ ) {
        for( c=0; c<(w-1); c++ )
            for( r=0; r<(w-1); r++ ) {
                calculateNormals(sphereVertexArray, normalArray, w, i, r, c);
            }
        calculateNormals(sphereVertexArray, normalArray, w, i, r, c);          // Calculate the last normal vector.
    }

    // Create Model and upload to GPU
    Model* model = LoadDataToModel(
            sphereVertexArray,
            normalArray,
            island->texCoordArray,
            NULL,
            island->indexArray,
            vertexCount,
            island->numIndices);


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
Model* generateIsland()
{
    int i, r, c;
    vec3 r_c_cube, r_c_sphere, r_c_ocean;
    float island_height;

    int w = ttex_island.width;
    float square_width = cube_width / (float)w;

    w += 1;             // need one extra vertex to scale from image properly

    int vertexCount = w*w*cube_sides;
    int triangleCount = (w-1) * (w-1) * 2 * cube_sides * 3;
    GLfloat *vertexArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
    GLfloat *normalArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
    GLfloat *texCoordArray = malloc(sizeof(GLfloat) * 2 * vertexCount);
    GLuint *indexArray = malloc(sizeof(GLuint) * triangleCount);
    sphereVertexArray = malloc(sizeof(GLfloat) * 3 * vertexCount);

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
                if (i==0) {     // only add height to the front side of the cube
                    island_height = generateIslandMask((float)w, (float)r, (float)c);
                    height_textureData = (ttex_island.imageData[(r + (c*w)) * (ttex_island.bpp/8)] / 50.0);
                    height = height_textureData * square_width * island_height;
                }

                // Transform cube to sphere positions
                r_c_sphere = Normalize(r_c_cube);

                // Add ocean vertices
                r_c_ocean = VectorAdd(r_c_sphere, ScalarMult(Normalize(r_c_sphere),height_ocean));
                sphereVertexArray[(r + (c*w) + (i*w*w)) * 3 + 0] = r_c_ocean.x;
                sphereVertexArray[(r + (c*w) + (i*w*w)) * 3 + 1] = r_c_ocean.y;
                sphereVertexArray[(r + (c*w) + (i*w*w)) * 3 + 2] = r_c_ocean.z;

                // Add height
                r_c_sphere = VectorAdd(r_c_sphere, ScalarMult(Normalize(r_c_sphere),height));


                vertexArray[(r + (c*w) + (i*w*w)) * 3 + 0] = r_c_sphere.x;
                vertexArray[(r + (c*w) + (i*w*w)) * 3 + 1] = r_c_sphere.y;
                vertexArray[(r + (c*w) + (i*w*w)) * 3 + 2] = r_c_sphere.z;


                calculateTextures(texCoordArray, w, i, r, c);
            }
        }


    for( i=0; i<cube_sides; i++ ) {
        for( c=0; c<(w-1); c++ )
            for( r=0; r<(w-1); r++ ) {
                calculateIndices(indexArray, w, i, r, c);
                calculateNormals(vertexArray, normalArray, w, i, r, c);
            }
        calculateNormals(vertexArray, normalArray, w, i, r, c);     // Calculate the last normal vector.
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

    return model;
}


void initIslandWorld(void)
{

    projectionMatrix_island = frustum(-0.001, 0.001, -0.001, 0.001, 0.002, 100.0);

    // Load shader for island
    program_island = loadShaders("islandWorld/island.vert", "islandWorld/island.frag");
    printError("init island shader");

    // Load texture
    glUniform1i(glGetUniformLocation(program_island, "tex"), 0); // Texture unit 0
    LoadTGATextureSimple("image/ground_sand_2.tga", &tex_sand);
    printError("init load texture");

    // Load terrain data
    LoadTGATextureData("image/fft-terrain.tga", &ttex_island);
    printError("init terrain data");

    // Generate planet
    island = generateIsland();
    ocean = generateOcean();
    printError("init planet model");

    // Load shader for ocean
    program_ocean = loadShaders("islandWorld/ocean.vert", "islandWorld/ocean.frag");
    printError("init ocean shader");


    identityMatrix_island = IdentityMatrix();

    // Model placement
    modelMatrix_island = S(1.0, 1.0, 1.0);
    modelMatrix_island = Mult(modelMatrix_island, Rz(1.8));
    modelMatrix_island = Mult(modelMatrix_island, Rx(1.1));
    modelMatrix_island = Mult(modelMatrix_island, Ry(-0.3));

    // Camera placement
    R_island = 1.05;
    verticalAngle_island = 0;
    horizontalAngle_island = 1;
    horizontalHeadAngle_island = 1.5;


}

void displayIslandWorld(void)
{
    // Handle keyboard
    keyboard(&R_island, &verticalAngle_island, &horizontalAngle_island, &horizontalHeadAngle_island);


    camMatrix_island = lookAt(
            R_island * cos(verticalAngle_island) * sin(horizontalAngle_island),
            R_island * sin(verticalAngle_island),
            R_island * cos(verticalAngle_island) * cos(horizontalAngle_island),

            0, horizontalHeadAngle_island, 0,

            0, 1, 0);



    // ---------------------------      Ground       ---------------------------

    glUseProgram(program_island);
    glUniformMatrix4fv(glGetUniformLocation(program_island, "projMatrix"), 1, GL_TRUE, projectionMatrix_island.m);
    glUniformMatrix4fv(glGetUniformLocation(program_island, "camMatrix"), 1, GL_TRUE, camMatrix_island.m);
    glUniformMatrix4fv(glGetUniformLocation(program_island, "identityMatrix"), 1, GL_TRUE, identityMatrix_island.m);
    glUniformMatrix4fv(glGetUniformLocation(program_island, "modelMatrix"), 1, GL_TRUE, modelMatrix_island.m);


    glBindTexture(GL_TEXTURE_2D, tex_sand);		// Bind Our Texture tex

    DrawModel(island, program_island, "inPosition", "inNormal", "inTexCoord");



    // ---------------------------      Ocean       ---------------------------

    glUseProgram(program_ocean);

    glUniformMatrix4fv(glGetUniformLocation(program_ocean, "projMatrix"), 1, GL_TRUE, projectionMatrix_island.m);
    glUniformMatrix4fv(glGetUniformLocation(program_ocean, "camMatrix"), 1, GL_TRUE, camMatrix_island.m);
    glUniformMatrix4fv(glGetUniformLocation(program_ocean, "identityMatrix"), 1, GL_TRUE, identityMatrix_island.m);
    glUniformMatrix4fv(glGetUniformLocation(program_ocean, "modelMatrix"), 1, GL_TRUE, modelMatrix_island.m);

    // Enable blending for transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Get current time
    time ( &rawtime );
    currentTime = localtime ( &rawtime );
    glUniform1f(glGetUniformLocation(program_ocean, "currentTime"), (float)currentTime->tm_sec);

    DrawModel(ocean, program_ocean, "inPosition", "inNormal", NULL);

    // Disable blending
    glDisable(GL_BLEND);

    // ---------------------------      Skybox       ---------------------------
    // Display skybox
    displaySkybox(projectionMatrix_island, camMatrix_island, identityMatrix_island);
    //displaySkybox(&R_island, &verticalAngle_island, &horizontalAngle_island, &horizontalHeadAngle_island);


}

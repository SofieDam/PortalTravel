#include <time.h>
#include <math.h>


mat4 projectionMatrix_island, identityMatrix_island, modelMatrix_island, camMatrix_island;

// vertex array object
Model *island, *ocean;

// Reference to shader program
GLuint program_island, program_ocean;

GLuint tex_sand;
TextureData ttex; // terrain

GLfloat *sphereVertexArray;

// LooakAt vertices
//float height=0, angle=0, zoom=1.0;

float R_island, verticalAngle_island, horizontalAngle_island, horizontalHeadAngle_island;

//vec3 eye, center, up;
//float speed;

// Time
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

    int vertexCount = island->numVertices;
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
            island->texCoordArray,
            NULL,
            island->indexArray,
            vertexCount,
            island->numIndices);


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
Model* generateIsland()
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
    LoadTGATextureData("image/fft-terrain.tga", &ttex);
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
    // Handle keyboard and update camera matrix
    //keyboard(&height, &angle, &zoom);
    //keyboard(&eye, &center, &up, speed);
    keyboard(&R_island, &verticalAngle_island, &horizontalAngle_island, &horizontalHeadAngle_island);

    // clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    //camMatrix = lookAt(zoom*sin(angle),zoom*height,zoom*cos(angle), 0,0,0, 0,1,0);

    camMatrix_island = lookAt(
            R_island * cos(verticalAngle_island) * sin(horizontalAngle_island),
            R_island * sin(verticalAngle_island),
            R_island * cos(verticalAngle_island) * cos(horizontalAngle_island),

            0, horizontalHeadAngle_island, 0,

            0, 1, 0);

    /*
    camMatrix = lookAt(eye.x, eye.y, eye.z,
                       center.x, center.y, center.z,
                       up.x, up.y, up.z);
                       */


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
    // http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-10-transparency/
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Get current time
    time ( &rawtime );
    currentTime = localtime ( &rawtime );
    glUniform1f(glGetUniformLocation(program_ocean, "currentTime"), (float)currentTime->tm_sec);

    DrawModel(ocean, program_ocean, "inPosition", "inNormal", NULL);

    // Disable blending
    glDisable(GL_BLEND);
}
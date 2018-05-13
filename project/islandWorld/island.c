/*
 * Create and draw everything connected to the island world.
 */

#include <time.h>

// Model
Model *island, *ocean, *boat, *palm, *shell1, *shell2, *fish;

// Shader program
GLuint program_island, program_ocean;

// Texture
GLuint tex_sand, tex_boat, tex_palm, tex_shell1, tex_shell2;

// Height-map
TextureData ttex_island;

// Matrices
mat4 projectionMatrix_island, identityMatrix_island, modelMatrix_island, camMatrix_island;
mat4 modelMatrix_object;

// Camera
float R_island, verticalAngle_island, horizontalAngle_island, horizontalHeadAngle_island;

// Time
time_t rawtime;
struct tm * currentTime;

// Sphere vertices for ocean
GLfloat *sphereVertexArray;
GLfloat *vertexArray;

// Height for ocean
const float height_ocean = 0.008;

// Fish position
float verticalAngle_fish, horizontalAngle_fish;

/*
 * Quadratic distance function.
 * Lower the height outside the circle.
 */
float generateIslandMask(float island_size, float r, float c)
{
    island_size = island_size * 0.5;
    float distance_r = fabs(r - island_size);
    float distance_c = fabs(c - island_size);
    float distance = sqrt(distance_r*distance_r + distance_c*distance_c);

    island_size = island_size - 10.0;
    float delta = distance / island_size;
    float grad = delta * delta;

    float result = fmax(0.0, (1.0 - grad));

    return result;
}

/*
 * Generate island planet
 */
void generateIslandPlanet()
{
    int i, r, c;
    vec3 r_c_cube, r_c_sphere, r_c_ocean;
    float island_height;

    int w = ttex_island.width;
    float square_width = cube_width / (float)w;

    w += 1;             // need one extra vertex to scale from image properly

    int vertexCount = w*w*cube_sides;
    int triangleCount = (w-1) * (w-1) * 2 * cube_sides * 3;
    vertexArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
    GLfloat *normalArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
    GLfloat *texCoordArray = malloc(sizeof(GLfloat) * 2 * vertexCount);
    GLuint *indexArray = malloc(sizeof(GLuint) * triangleCount);

    sphereVertexArray = malloc(sizeof(GLfloat) * 3 * vertexCount); // Sphere vertices for ocean

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

                // Add height to island
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

    island = LoadDataToModel(
            vertexArray,
            normalArray,
            texCoordArray,
            NULL,
            indexArray,
            vertexCount,
            triangleCount);

    ocean = LoadDataToModel(
            sphereVertexArray,
            sphereVertexArray,
            texCoordArray,
            NULL,
            indexArray,
            vertexCount,
            triangleCount);
}


void initIslandWorld(void)
{
    // Load shader for island
    program_island = loadShaders("islandWorld/island.vert", "islandWorld/island.frag");

    // Load texture
    glUniform1i(glGetUniformLocation(program_island, "tex"), 0);
    LoadTGATextureSimple("image/ground_sand_2.tga", &tex_sand);
    LoadTGATextureSimple("image/wood.tga", &tex_boat);

    // Load object
    boat = LoadModelPlus("object/boat.obj");

    // Load terrain data
    LoadTGATextureData("image/fft-terrain.tga", &ttex_island);

    // Load Palm trees - model and textures
    palm = LoadModelPlus("object/palm.obj");
    LoadTGATextureSimple("image/palm.tga", &tex_palm);

    // Load Palm trees - model and textures
    shell1 = LoadModelPlus("object/shell1.obj");
    shell2 = LoadModelPlus("object/shell2.obj");
    LoadTGATextureSimple("image/shell1.tga", &tex_shell1);
    LoadTGATextureSimple("image/shell2.tga", &tex_shell2);

    // Load fish
    fish = LoadModelPlus("object/fish.obj");

    // Generate planet
    generateIslandPlanet();

    // Load shader for ocean
    program_ocean = loadShaders("islandWorld/ocean.vert", "islandWorld/ocean.frag");

    // Matrices
    projectionMatrix_island = frustum(-0.001, 0.001, -0.001, 0.001, 0.002, 10.0);
    identityMatrix_island = IdentityMatrix();
    modelMatrix_island = S(1.0, 1.0, 1.0);
    modelMatrix_island = Mult(modelMatrix_island, Rz(1.8));
    modelMatrix_island = Mult(modelMatrix_island, Rx(1.1));
    modelMatrix_island = Mult(modelMatrix_island, Ry(-0.3));

    // Camera placement
    R_island = 1.1;
    verticalAngle_island = 0;
    horizontalAngle_island = 0.55;
    horizontalHeadAngle_island = 1.7;

    // Fish
    verticalAngle_fish = 0;
    horizontalAngle_fish = 0;

}

// Calculate object position
void objectPosition_island(int i, int c, int r, int w, float scale, mat4 *modelMatrix, GLfloat *vertices)
{
    vec3 position = SetVector(vertices[(r + (c * w) + (i * w * w)) * 3 + 0],
                              vertices[(r + (c * w) + (i * w * w)) * 3 + 1],
                              vertices[(r + (c * w) + (i * w * w)) * 3 + 2]);

    mat4 rotationMatrix = calculateObjectMatrix(position);

    *modelMatrix = Mult(rotationMatrix, (S(scale, scale, scale)));
}

void drawFish(int yAxis, float r_fish)
{
    vec3 objectPosition;
    float v = 0;

    int i;
    for (i = 0; i < 6; ++i)
    {
        // Rotate around y-axis
        if (yAxis == 1) {
            objectPosition = SetVector(r_fish * cos(0) * cos(horizontalAngle_fish + v),
                                       r_fish * sin(0),
                                       r_fish * cos(0) * sin(horizontalAngle_fish + v));
        }
            // Rotate around x-axis
        else
        {
            objectPosition = SetVector(r_fish * sin(verticalAngle_fish + v),
                                       r_fish * cos(verticalAngle_fish + v) * cos(0),
                                       r_fish * cos(verticalAngle_fish + v) * sin(0));
        }


        mat4 rotationMatrix = calculateObjectMatrix(objectPosition);


        rotationMatrix = Mult(rotationMatrix, Ry(-M_PI/2));

        modelMatrix_object = Mult(rotationMatrix, (S(0.005, 0.003, 0.005)));

        glUniformMatrix4fv(glGetUniformLocation(program_island, "modelMatrix"), 1, GL_TRUE, modelMatrix_object.m);
        glBindTexture(GL_TEXTURE_2D, tex_shell2);
        DrawModel(fish, program_island, "inPosition", "inNormal", "inTexCoord");

        v += 0.05;
    }
}

void displayIslandWorld(void)
{
    // Handle keyboard
    keyboard(&R_island, &verticalAngle_island, &horizontalAngle_island, &horizontalHeadAngle_island);

    // Update camera matrix
    camMatrix_island = lookAt(
            R_island * cos(verticalAngle_island) * cos(horizontalAngle_island),
            R_island * sin(verticalAngle_island),
            R_island * cos(verticalAngle_island) * sin(horizontalAngle_island),

            0, horizontalHeadAngle_island, 0,

            0, 1, 0);

    // ---------------------------      Skybox       ---------------------------
    displaySkybox(projectionMatrix_island, camMatrix_island, identityMatrix_island);


    // ---------------------------      Ground       ---------------------------

    glUseProgram(program_island);
    glUniformMatrix4fv(glGetUniformLocation(program_island, "projMatrix"), 1, GL_TRUE, projectionMatrix_island.m);
    glUniformMatrix4fv(glGetUniformLocation(program_island, "camMatrix"), 1, GL_TRUE, camMatrix_island.m);
    glUniformMatrix4fv(glGetUniformLocation(program_island, "identityMatrix"), 1, GL_TRUE, identityMatrix_island.m);
    glUniformMatrix4fv(glGetUniformLocation(program_island, "modelMatrix"), 1, GL_TRUE, modelMatrix_island.m);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex_sand);

    DrawModel(island, program_island, "inPosition", "inNormal", "inTexCoord");

    // ---------------------------      Object       ---------------------------

    int w = ttex_island.width + 1;
    int i, c, r;
    for( i=0; i<cube_sides; i++ )
        for( c=0; c<w; c++ ) {
            for (r = 0; r < w; r++) {

                if (i == 2)
                {
                    if ((c==100) && (r==100))
                    {
                        // Floating boat
                        objectPosition_island(i, c, r, w, 0.005, &modelMatrix_object, sphereVertexArray);
                        glUniformMatrix4fv(glGetUniformLocation(program_island, "modelMatrix"), 1, GL_TRUE, modelMatrix_object.m);
                        glBindTexture(GL_TEXTURE_2D, tex_boat);
                        DrawModel(boat, program_island, "inPosition", "inNormal", "inTexCoord");
                    }
                }


                if ((i == 4) || (i == 3))
                {
                    if ((80 < c) && (c < 100) && (80 < r) && (r < 100))
                        if (((c % 8) == 0) && ((r % 8) == 0)) {

                            // Boat at bottom
                            objectPosition_island(i, c, r, w, 0.002, &modelMatrix_object, vertexArray);
                            glUniformMatrix4fv(glGetUniformLocation(program_island, "modelMatrix"), 1, GL_TRUE, modelMatrix_object.m);
                            glBindTexture(GL_TEXTURE_2D, tex_boat);
                            DrawModel(boat, program_island, "inPosition", "inNormal", "inTexCoord");
                        }
                }

                if ((i == 0))
                {
                    if ((39 < c) && (c < 160) && (200 < r) && (r < w)) {
                        if (((c % 40) == 0) && ((r % 32) == 0)) {

                            // Palm
                            objectPosition_island(i, c+45, r, w, 0.00005, &modelMatrix_object, sphereVertexArray);
                            glUniformMatrix4fv(glGetUniformLocation(program_island, "modelMatrix"), 1, GL_TRUE,
                                               modelMatrix_object.m);
                            glBindTexture(GL_TEXTURE_2D, tex_palm);
                            DrawModel(palm, program_island, "inPosition", "inNormal", "inTexCoord");
                        }
                    }
                }
                if (i == 1)
                {
                    if (((64 < c) && (c < 161) && (0 < r) && (r < 50)) ||
                        ((50 < c) && (c < 161) && (70 < r) && (r < 100)) ||
                        ((10 < c) && (c < 140) && (50 < r) && (r < 70)) ||
                        ((70 < c) && (c < 161) && (100 < r) && (r < 130))) {
                        if (((c % 35) == 0) && ((r % 32) == 0)) {
                            // Palm
                            objectPosition_island(i, c, r, w, 0.00005, &modelMatrix_object, sphereVertexArray);
                            glUniformMatrix4fv(glGetUniformLocation(program_island, "modelMatrix"), 1, GL_TRUE,
                                               modelMatrix_object.m);
                            glBindTexture(GL_TEXTURE_2D, tex_palm);
                            DrawModel(palm, program_island, "inPosition", "inNormal", "inTexCoord");
                        }
                    }
                }
                if (i == 0)
                {
                    //if (((100 < c) && (c < 180) && (0 < r) && (r < 130))) {
                        if (((c % 50) == 0) && ((r % 50) == 0)) {
                            // Shell1
                            objectPosition_island(i, c, r, w, 0.0002, &modelMatrix_object, vertexArray);
                            glUniformMatrix4fv(glGetUniformLocation(program_island, "modelMatrix"), 1, GL_TRUE,
                                               modelMatrix_object.m);
                            glBindTexture(GL_TEXTURE_2D, tex_shell1);
                            DrawModel(shell1, program_island, "inPosition", "inNormal", "inTexCoord");

                            // Shell2
                            objectPosition_island(i, c + 5, r - 5, w, 0.0003, &modelMatrix_object, vertexArray);
                            glUniformMatrix4fv(glGetUniformLocation(program_island, "modelMatrix"), 1, GL_TRUE,
                                               modelMatrix_object.m);
                            glBindTexture(GL_TEXTURE_2D, tex_shell2);
                            DrawModel(shell2, program_island, "inPosition", "inNormal", "inTexCoord");
                        }

                    //}


                }
            }
        }

    drawFish(1, 1.004);
    drawFish(0, 1.004);
    horizontalAngle_fish += 0.001;   // update fish position horizontally
    verticalAngle_fish += 0.001;     // update fish position vertically

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



}

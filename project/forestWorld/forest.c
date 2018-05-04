/*
 * Create and draw everything connected to the forest world.
 */

mat4 projectionMatrix_forest, identityMatrix_forest, modelMatrix_forest, camMatrix_forest, objectMatrix;

// Vertex array for sphere
GLfloat *vertexArray;

// vertex array object
Model *forest, *tree_fir, *maple_leaves, *maple_stem, *stump, *rock, *eagle;

// Reference to shader program
GLuint program_forest, program_tree;

GLuint tex_grass, texBranch, texMapleLeaves, texMapleStem, texStump, texRock, texPalm;
TextureData ttex_forest; // terrain

// Camera
float R_forest, verticalAngle_forest, horizontalAngle_forest, horizontalHeadAngle_forest;

// Bird position
float verticalAngle_bird, horizontalAngle_bird;


/*
 * 1. Build a cube.
 * 2. Normalize every vertex which will result in a sphere.
 */
Model* generateForestPlanet()
{
    int i, r, c;
    vec3 r_c_cube, r_c_sphere;

    int w = ttex_forest.width;
    float square_width = cube_width / (float)w;

    w += 1;             // need one extra vertex to scale from image properly

    int vertexCount = w*w*cube_sides;
    int triangleCount = (w-1) * (w-1) * 2 * cube_sides * 3;

    vertexArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
    GLfloat *texCoordArray = malloc(sizeof(GLfloat) * 2 * vertexCount);
    GLuint *indexArray = malloc(sizeof(GLuint) * triangleCount);

    // Calculate vertices
    for( i=0; i<cube_sides; i++ )
        for( c=0; c<w; c++ ) {
            for( r=0; r<w; r++ ) {

                // Ground
                r_c_cube = SetVector(r, c, 0);
                r_c_cube = ScalarMult(r_c_cube, square_width);
                r_c_cube = MultMat3Vec3(cube_xyz_plane[i],r_c_cube);
                r_c_cube = VectorAdd(cube_vertices[i], r_c_cube);

                // Transform cube to sphere positions
                r_c_sphere = Normalize(r_c_cube);

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
            }
    }



// Create Model and upload to GPU
    Model* model = LoadDataToModel(
            vertexArray,
            vertexArray,
            texCoordArray,
            NULL,
            indexArray,
            vertexCount,
            triangleCount);

    return model;
}

// Calculate object position
void objectPosition_forest(int i, int c, int r, int w, float scaleX, float scaleY, float scaleZ)
{
    vec3 objectPosition = SetVector(vertexArray[(r + (c * w) + (i * w * w)) * 3 + 0],
                             vertexArray[(r + (c * w) + (i * w * w)) * 3 + 1],
                             vertexArray[(r + (c * w) + (i * w * w)) * 3 + 2]);

    vec3 y = Normalize(objectPosition);

    vec3 x_hat = {1, 0, 0};
    x_hat = Normalize(x_hat);

    vec3 z = Normalize(CrossProduct(x_hat, y));
    vec3 x = Normalize(CrossProduct(y, z));

    mat4 rotationMatrix = {{       x.x, y.x, z.x, objectPosition.x,
                                   x.y, y.y, z.y, objectPosition.y,
                                   x.z, y.z, z.z, objectPosition.z,
                                   0.0, 0.0, 0.0, 1.0}};

    objectMatrix = Mult(rotationMatrix, (S(scaleX, scaleY,scaleZ)));
    glUniformMatrix4fv(glGetUniformLocation(program_tree, "modelMatrix"), 1, GL_TRUE, objectMatrix.m);
}

void initForestWorld(void)
{
    projectionMatrix_forest = frustum(-0.001, 0.001, -0.001, 0.001, 0.002, 10.0);

    // Load shader
    program_forest = loadShaders("forestWorld/forest.vert", "forestWorld/forest.frag");
    program_tree = loadShaders("forestWorld/tree.vert", "forestWorld/tree.frag");

    // Load texture
    glUniform1i(glGetUniformLocation(program_forest, "tex"), 0); // Texture unit 0
    LoadTGATextureSimple("image/grass_1.tga", &tex_grass);

    // Load terrain data
    LoadTGATextureData("image/fft-terrain.tga", &ttex_forest);

    // Generate planet
    forest = generateForestPlanet();

    //Load maple trees - model and textures
    maple_leaves = LoadModelPlus("object/MapleTreeLeaves.obj");
    maple_stem = LoadModelPlus("object/MapleTreeStem.obj");
    glUniform1i(glGetUniformLocation(program_tree, "texTree"), 1); // Texture unit 1
    LoadTGATextureSimple("image/maple_leaf.tga", &texMapleLeaves);
    LoadTGATextureSimple("image/bark.tga", &texMapleStem);

    // Load firs - model and textures
    tree_fir = LoadModelPlus("object/fir.obj");
    LoadTGATextureSimple("image/branch.tga", &texBranch);
    //LoadTGATextureSimple("image/bark.tga", &texStem);

    // Load stumps - model and textures
    stump = LoadModelPlus("object/stump.obj");
    LoadTGATextureSimple("image/stump.tga", &texStump);

    // Load rocks - model and textures
    rock = LoadModelPlus("object/Rock.obj");
    LoadTGATextureSimple("image/Rock.tga", &texRock);

    // Eagle
    eagle = LoadModelPlus("object/eagle.obj");


    // Matrices
    identityMatrix_forest = IdentityMatrix();
    modelMatrix_forest = S(1.0, 1.0, 1.0);


    // Camera placement
    R_forest = 1.05;
    verticalAngle_forest = 0;
    horizontalAngle_forest = 1;
    horizontalHeadAngle_forest = 1.7;

    //R_forest = 1.05;
    /*
    R_forest = 3.0;
    verticalAngle_forest = 0;
    horizontalAngle_forest = 1;
    horizontalHeadAngle_forest = 1.5;
     */

    // Bird
    verticalAngle_bird = 0;
    horizontalAngle_bird = 0;

}

void drawBird(int yAxis, float r_bird)
{
    vec3 objectPosition;

    // Rotate around y-axis
    if (yAxis == 1) {
        objectPosition = SetVector(r_bird * cos(0) * cos(horizontalAngle_bird),
                                   r_bird * sin(0),
                                   r_bird * cos(0) * sin(horizontalAngle_bird));
    }
    // Rotate around x-axis
    else
    {
        objectPosition = SetVector(r_bird * sin(verticalAngle_bird),
                                   r_bird * cos(verticalAngle_bird) * cos(0),
                                   r_bird * cos(verticalAngle_bird) * sin(0));
    }


    vec3 y = Normalize(objectPosition);

    vec3 x_hat = {1, 0, 0};
    x_hat = Normalize(x_hat);

    vec3 z = Normalize(CrossProduct(x_hat, y));
    vec3 x = Normalize(CrossProduct(y, z));

    mat4 rotationMatrix = {{       x.x, y.x, z.x, objectPosition.x,
                                   x.y, y.y, z.y, objectPosition.y,
                                   x.z, y.z, z.z, objectPosition.z,
                                   0.0, 0.0, 0.0, 1.0}};

    rotationMatrix = Mult(rotationMatrix, Ry(-M_PI/2));

    objectMatrix = Mult(rotationMatrix, (S(0.3, 0.3, 0.3)));

    glUniformMatrix4fv(glGetUniformLocation(program_tree, "modelMatrix"), 1, GL_TRUE, objectMatrix.m);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texStump);
    DrawModel(eagle, program_tree, "inPosition", "inNormal", "inTexCoord");
}

void displayForestWorld(void)
{
    // Handle keyboard
    keyboard(&R_forest, &verticalAngle_forest, &horizontalAngle_forest, &horizontalHeadAngle_forest);

    // Update camera matrix
    camMatrix_forest = lookAt(
            R_forest * cos(verticalAngle_forest) * cos(horizontalAngle_forest),
            R_forest * sin(verticalAngle_forest),
            R_forest * cos(verticalAngle_forest) * sin(horizontalAngle_forest),

            0, horizontalHeadAngle_forest, 0,

            0, 1, 0);

    // ---------------------------      Skybox       ---------------------------
    displaySkybox(projectionMatrix_forest, camMatrix_forest, identityMatrix_forest);


    // ---------------------------      Ground       ---------------------------

    glUseProgram(program_forest);
    glUniformMatrix4fv(glGetUniformLocation(program_forest, "projMatrix"), 1, GL_TRUE, projectionMatrix_forest.m);
    glUniformMatrix4fv(glGetUniformLocation(program_forest, "camMatrix"), 1, GL_TRUE, camMatrix_forest.m);
    glUniformMatrix4fv(glGetUniformLocation(program_forest, "identityMatrix"), 1, GL_TRUE, identityMatrix_forest.m);
    glUniformMatrix4fv(glGetUniformLocation(program_forest, "modelMatrix"), 1, GL_TRUE, modelMatrix_forest.m);

    glBindTexture(GL_TEXTURE_2D, tex_grass);

    DrawModel(forest, program_forest, "inPosition", "inNormal", "inTexCoord");


    // ---------------------------      Objects       ---------------------------
    glUseProgram(program_tree);
    glUniformMatrix4fv(glGetUniformLocation(program_tree, "projMatrix"), 1, GL_TRUE, projectionMatrix_forest.m);
    glUniformMatrix4fv(glGetUniformLocation(program_tree, "camMatrix"), 1, GL_TRUE, camMatrix_forest.m);
    glUniformMatrix4fv(glGetUniformLocation(program_tree, "identityMatrix"), 1, GL_TRUE, identityMatrix_forest.m);

    // Activate transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //int i = 0;
    int i = 4;
    int c = 0;
    int r = 0;
    int w = ttex_forest.width + 1;
    //for( i=0; i<cube_sides; i++ )
        for( c=0; c<w; c++ ) {
            for( r=0; r<w; r++ ) {

                // Draw Maple trees
                if (((c % 27) == 0) && ((r % 27) == 0)) {
                    if( ((c + 15) <= w ) && ((r + 15) <= w) ) {
                        objectPosition_forest(i, (c + 15), (r + 15), w, 0.003, 0.005, 0.003);

                        glUniform1i(glGetUniformLocation(program_tree, "texSample"), 1);

                        glActiveTexture(GL_TEXTURE1);
                        glBindTexture(GL_TEXTURE_2D, texMapleLeaves);
                        DrawModel(maple_leaves, program_tree, "inPosition", "inNormal", "inTexCoord");

                        glBindTexture(GL_TEXTURE_2D, texMapleStem);
                        DrawModel(maple_stem, program_tree, "inPosition", "inNormal", "inTexCoord");
                    }
                }

                // Draw firs
                if (((c % 27) == 0) && ((r % 27) == 0)) {
                    objectPosition_forest(i, c, r, w, 0.03, 0.05, 0.03);
                    glUniform1i(glGetUniformLocation(program_tree, "texSample"), 1);

                    glActiveTexture(GL_TEXTURE1);
                    glBindTexture(GL_TEXTURE_2D, texBranch);
                    DrawModel(tree_fir, program_tree, "inPosition", "inNormal", "inTexCoord");
                }

                // Draw stumps
                if (((c % 42) == 0) && ((r % 42) == 0)) {
                    objectPosition_forest(i, c, r, w, 0.01, 0.01, 0.01);
                    glUniform1i(glGetUniformLocation(program_tree, "texSample"), 1);

                    glActiveTexture(GL_TEXTURE1);
                    glBindTexture(GL_TEXTURE_2D, texStump);
                    DrawModel(stump, program_tree, "inPosition", "inNormal", "inTexCoord");
                }

                // Draw rocks
                if (((c % 52) == 0) && ((r % 52) == 0)) {
                    objectPosition_forest(i, c, r, w, 0.0003, 0.0003, 0.0003);
                    glUniform1i(glGetUniformLocation(program_tree, "texSample"), 1);

                    glActiveTexture(GL_TEXTURE1);
                    glBindTexture(GL_TEXTURE_2D, texRock);
                    DrawModel(rock, program_tree, "inPosition", "inNormal", "inTexCoord");
                }


            }
        }

    glDisable(GL_BLEND); // Disable blending

    // Bird
    drawBird(1, 1.2);
    drawBird(0, 1.5);
    horizontalAngle_bird += 0.01;   // update bird position horizontally
    verticalAngle_bird += 0.01;     // update bird position vertically
    }
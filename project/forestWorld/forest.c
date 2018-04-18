

mat4 projectionMatrix_forest, identityMatrix_forest, modelMatrix_forest, camMatrix_forest, treeMatrix;

// vertex array object
Model *forest, *tree;

// Reference to shader program
GLuint program_forest, program_tree;

GLuint tex_grass;
TextureData ttex_forest; // terrain

//vec3 treeArray[];
vec3 treeArray[6];


float R_forest, verticalAngle_forest, horizontalAngle_forest, horizontalHeadAngle_forest;

/*
 * 1. Build a cube.
 * 2. Normalize every vertex which will result in a sphere.
 */
Model* generateForest()
{
    int i, r, c;
    vec3 r_c_cube, r_c_sphere;

    int w = ttex_forest.width;
    float square_width = cube_width / (float)w;

    w += 1;             // need one extra vertex to scale from image properly

    int vertexCount = w*w*cube_sides;
    int triangleCount = (w-1) * (w-1) * 2 * cube_sides * 3;

    GLfloat *vertexArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
    GLfloat *normalArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
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


                if ((c == 120) && (r == 120)) {
                    treeArray[i] = r_c_sphere;
                    //printf("x=%f, y=%f, z=%f\n", treeArray[i].x, treeArray[i].y, treeArray[i].z);
                }

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


void initForestWorld(void)
{
    projectionMatrix_forest = frustum(-0.001, 0.001, -0.001, 0.001, 0.002, 100.0);

    // Load shader for island
    program_forest = loadShaders("forestWorld/forest.vert", "forestWorld/forest.frag");
    printError("init forest shader");

    // Load texture
    glUniform1i(glGetUniformLocation(program_forest, "tex"), 0); // Texture unit 0
    LoadTGATextureSimple("image/grass_1.tga", &tex_grass);
    printError("init load texture");

    // Load terrain data
    LoadTGATextureData("image/fft-terrain.tga", &ttex_forest);
    printError("init terrain data");

    // Generate planet
    forest = generateForest();
    printError("init planet model");

    program_tree = loadShaders("forestWorld/tree.vert", "forestWorld/tree.frag");
    printError("init tree shader");
    tree = LoadModelPlus("object/Tree.obj");



    identityMatrix_forest = IdentityMatrix();

    // Model placement
    modelMatrix_forest = S(1.0, 1.0, 1.0);



    //treeMatrix = T(treeArray.x, treeArray.y, treeArray.z);

    //vec3 n = {0,1,0};
    //n = Normalize(n);
    //n = VectorSub(treeArray,n);

/*
    treeMatrix = Mult(treeMatrix,Rx(acos(DotProduct(n,treeArray.x))));
    treeMatrix = Mult(treeMatrix,Ry(acos(DotProduct(n,treeArray.y))));
    treeMatrix = Mult(treeMatrix,Rz(acos(DotProduct(n,treeArray.z))));
*/

/*
    treeMatrix = Mult(treeMatrix,Rx(treeArray.x));
    treeMatrix = Mult(treeMatrix,Ry(treeArray.y));
    treeMatrix = Mult(treeMatrix,Rz(treeArray.z));
*/


    // i=0, x=-0.571144, y=-0.571144, z=0.589568
    /*
    vec3 up = {0,1,0};
    up = Normalize(up);
    GLfloat angle = DotProduct(up,treeArray);
    float s = 1 / (sqrt( (1+angle)*2 ));
    vec3 rotation = ScalarMult((CrossProduct(up,treeArray)), s);

    treeMatrix = Mult(treeMatrix,Rx(acos(rotation.x)));
    treeMatrix = Mult(treeMatrix,Ry(acos(rotation.y)));
    treeMatrix = Mult(treeMatrix,Rz(acos(rotation.z)));
     */



    // acos
    // i = 1, x=0.589568, y=-0.571144, z=0.571144
    // i = 5, x=0.571144, y=-0.589568, z=0.571144
    /*
    treeMatrix = Mult(treeMatrix,Rx(acosf(treeArray.x)));
    treeMatrix = Mult(treeMatrix,Ry(acosf(treeArray.y)));
    treeMatrix = Mult(treeMatrix,Rz(acosf(treeArray.z)));
     */



    //treeMatrix = Mult(treeMatrix,Rx(atanf(treeArray.x/n.x)));
    //treeMatrix = Mult(treeMatrix,Ry(atanf(treeArray.y/n.y)));
    //treeMatrix = Mult(treeMatrix,Rz(atanf(treeArray.z/n.z)));



    //treeMatrix = Mult(treeMatrix,(S(0.05, 0.05, 0.05)));


    //treeMatrix = T(n.x, n.y, n.z);


    // Camera placement
    R_forest = 3.0;
    verticalAngle_forest = 0;
    horizontalAngle_forest = 1;
    horizontalHeadAngle_forest = 0;

    //R_forest = 1.05;
    /*
    R_forest = 3.0;
    verticalAngle_forest = 0;
    horizontalAngle_forest = 1;
    horizontalHeadAngle_forest = 1.5;
     */


}


void displayForestWorld(void)
{
    // Handle keyboard
    keyboard(&R_forest, &verticalAngle_forest, &horizontalAngle_forest, &horizontalHeadAngle_forest);


    camMatrix_forest = lookAt(
            R_forest * cos(verticalAngle_forest) * sin(horizontalAngle_forest),
            R_forest * sin(verticalAngle_forest),
            R_forest * cos(verticalAngle_forest) * cos(horizontalAngle_forest),

            0, horizontalHeadAngle_forest, 0,

            0, 1, 0);

    // ---------------------------      Skybox       ---------------------------
    // Display skybox
    displaySkybox(projectionMatrix_forest, camMatrix_forest, identityMatrix_forest);


    // ---------------------------      Ground       ---------------------------
    glUseProgram(program_forest);
    glUniformMatrix4fv(glGetUniformLocation(program_forest, "projMatrix"), 1, GL_TRUE, projectionMatrix_forest.m);
    glUniformMatrix4fv(glGetUniformLocation(program_forest, "camMatrix"), 1, GL_TRUE, camMatrix_forest.m);
    glUniformMatrix4fv(glGetUniformLocation(program_forest, "identityMatrix"), 1, GL_TRUE, identityMatrix_forest.m);
    glUniformMatrix4fv(glGetUniformLocation(program_forest, "modelMatrix"), 1, GL_TRUE, modelMatrix_forest.m);

    glBindTexture(GL_TEXTURE_2D, tex_grass);		// Bind Our Texture tex

    DrawModel(forest, program_forest, "inPosition", "inNormal", "inTexCoord");

    // ---------------------------      Trees       ---------------------------
    glUseProgram(program_tree);
    glUniformMatrix4fv(glGetUniformLocation(program_tree, "projMatrix"), 1, GL_TRUE, projectionMatrix_forest.m);
    glUniformMatrix4fv(glGetUniformLocation(program_tree, "camMatrix"), 1, GL_TRUE, camMatrix_forest.m);
    glUniformMatrix4fv(glGetUniformLocation(program_tree, "identityMatrix"), 1, GL_TRUE, identityMatrix_forest.m);

    int i = 0;

    for( i=0; i<6; i++ ) {

        treeMatrix = T(treeArray[i].x, treeArray[i].y, treeArray[i].z);

/*
        treeMatrix = Mult(treeMatrix,Rx(cos(treeArray[i].x)));
        treeMatrix = Mult(treeMatrix,Ry(cos(treeArray[i].y)));
        treeMatrix = Mult(treeMatrix,Rz(cos(treeArray[i].z)));
        */


 // http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-17-quaternions/
        vec3 up = {0,1,0};
        up = Normalize(up);
        GLfloat angle = DotProduct(up,treeArray[i]);

        float s = 1 / (sqrt( (1+angle)*2 ));
        vec3 rotation = ScalarMult((CrossProduct(up,treeArray[i])), s);

        treeMatrix = Mult(treeMatrix,Rx(rotation.x));
        treeMatrix = Mult(treeMatrix,Ry(rotation.y));
        treeMatrix = Mult(treeMatrix,Rz(rotation.z));



        treeMatrix = Mult(treeMatrix,(S(0.05, 0.05, 0.05)));



        glUniformMatrix4fv(glGetUniformLocation(program_tree, "modelMatrix"), 1, GL_TRUE, treeMatrix.m);

        glBindTexture(GL_TEXTURE_2D, tex_grass);		// Bind Our Texture tex
        DrawModel(tree, program_tree, "inPosition", "inNormal", "inTexCoord");
    }

}

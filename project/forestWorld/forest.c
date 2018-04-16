

mat4 projectionMatrix, identityMatrix, modelMatrix, camMatrix;

// vertex array object
Model *forest;

// Reference to shader program
GLuint program_forest;

GLuint tex_grass;
TextureData ttex; // terrain


float R, verticalAngle, horizontalAngle, horizontalHeadAngle;

/*
 * 1. Build a cube.
 * 2. Normalize every vertex which will result in a sphere.
 * 3. Add height (from greyscale-image) to sphere.
 */
Model* generateForest()
{
    /*
    int i, r, c;
    vec3 r_c_cube, r_c_sphere, r_c_ocean;
    float island_height;

    int w = ttex.width;


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
     */
    return NULL;
}

void initForestWorld(void)
{
    projectionMatrix = frustum(-0.001, 0.001, -0.001, 0.001, 0.002, 100.0);

    // Load shader for island
    program_forest = loadShaders("forestWorld/forest.vert", "forestWorld/forest.frag");
    printError("init forest shader");

    // Load texture
    glUniform1i(glGetUniformLocation(program_forest, "tex"), 0); // Texture unit 0
    LoadTGATextureSimple("image/grass.tga", &tex_grass);
    printError("init load texture");

    // Load terrain data
    LoadTGATextureData("image/fft-terrain.tga", &ttex);
    printError("init terrain data");

    // Generate planet
    forest = generateForest();
    printError("init planet model");


    //identityMatrix = IdentityMatrix();

    // Model placement
    /*
    modelMatrix = S(1.0, 1.0, 1.0);
    modelMatrix = Mult(modelMatrix, Rz(1.8));
    modelMatrix = Mult(modelMatrix, Rx(1.1));
    modelMatrix = Mult(modelMatrix, Ry(-0.3));
     */

    // Camera placement
    /*
    R = 5;
    verticalAngle = 0;
    horizontalAngle = 1;
    horizontalHeadAngle = 1;
     */

    /*
    R = 1.05;
    verticalAngle = 0;
    horizontalAngle = 1;
    horizontalHeadAngle = 1.5;
     */

}

void displayForestWorld(void)
{

}

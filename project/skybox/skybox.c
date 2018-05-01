// Select texture set by setting this constant to 0 or 6
#define TEXTURE_OFFSET 0

Model *skybox_side[6];

GLuint program_skybox;

// Skybox
GLuint cubemap;
TextureData skyboxTex[6];

mat4 cameraMatrix_skybox, projectionMatrix_skybox;

GLfloat skybox_vertices[6][4*3] =
        {
                { // +x
                        0.5,-0.5,-0.5,		// 1
                        0.5,0.5,-0.5,		// 2
                        0.5,0.5,0.5,			// 6
                        0.5,-0.5,0.5,		// 5
                },
                { // -x
                        -0.5,-0.5,-0.5,		// 0 -0
                        -0.5,-0.5,0.5,		// 4 -1
                        -0.5,0.5,0.5,		// 7 -2
                        -0.5,0.5,-0.5,		// 3 -3
                },
                { // +y
                        0.5,0.5,-0.5,		// 2 -0
                        -0.5,0.5,-0.5,		// 3 -1
                        -0.5,0.5,0.5,		// 7 -2
                        0.5,0.5,0.5,			// 6 -3
                },
                { // -y
                        -0.5,-0.5,-0.5,		// 0
                        0.5,-0.5,-0.5,		// 1
                        0.5,-0.5,0.5,		// 5
                        -0.5,-0.5,0.5		// 4
                },
                { // +z
                        -0.5,-0.5,0.5,		// 4
                        0.5,-0.5,0.5,		// 5
                        0.5,0.5,0.5,			// 6
                        -0.5,0.5,0.5,		// 7
                },
                { // -z
                        -0.5,-0.5,-0.5,	// 0
                        -0.5,0.5,-0.5,		// 3
                        0.5,0.5,-0.5,		// 2
                        0.5,-0.5,-0.5,		// 1
                }
        };

GLfloat skybox_texcoord[6][4*2] =
        {
                {
                        // Left
                        1.0, 1.0,
                        1.0, 0.0,
                        0.0, 0.0,
                        0.0, 1.0,
                },
                {
                        // Right
                        0.0, 1.0,
                        1.0, 1.0,
                        1.0, 0.0,
                        0.0, 0.0,
                },
                {
                        // Top
                        1.0, 0.0,
                        0.0, 0.0,
                        0.0, 1.0,
                        1.0, 1.0,
                },
                {
                        // Bottom
                        0.0, 1.0,
                        1.0, 1.0,
                        1.0, 0.0,
                        0.0, 0.0,
                },
                {
                        // Back
                        0.0, 1.0,
                        1.0, 1.0,
                        1.0, 0.0,
                        0.0, 0.0,
                },
                {
                        // Front
                        1.0, 1.0,
                        1.0, 0.0,
                        0.0, 0.0,
                        0.0, 1.0,
                }
        };
GLuint skybox_indices[6][6] =
        {
                {0, 2, 1, 0, 3, 2},
                {0, 2, 1, 0, 3, 2},
                {0, 2, 1, 0, 3, 2},
                {0, 2, 1, 0, 3, 2},
                {0, 2, 1, 0, 3, 2},
                {0, 2, 1, 0, 3, 2}
        };



char *textureFileName[6] =
        {

                "image/sky.tga",   //left
                "image/sky.tga",   //right
                "image/sky.tga",   //top
                "image/sky.tga",   //botton
                "image/sky.tga",   //back
                "image/sky.tga",   //front

        };


void initSkybox()
{
    program_skybox = loadShaders("skybox/skybox.vert", "skybox/skybox.frag");
    glUseProgram(program_skybox);


    int i;
    for (i = 0; i < 6; i++)
    {
        skybox_side[i] = LoadDataToModel(
                skybox_vertices[i],
                NULL,
                skybox_texcoord[i],
                NULL,
                skybox_indices[i],
                4,
                6);
        //printf("Loading texture data %d done\n", i);
        // printf("Loading texture %s\n", textureFileName[i+TEXTURE_OFFSET]);

        LoadTGATexture(textureFileName[i+TEXTURE_OFFSET], &skyboxTex[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    glUniform1i(glGetUniformLocation(program_skybox, "tex"), 0); // Texture unit 0

    //loadSkyboxTextures();
}


void displaySkybox(mat4 projectionMatrix, mat4 cameraMatrix, mat4 identityMatrix)
{
    int i;

    glUseProgram(program_skybox);

    // draw box
    glDisable(GL_DEPTH_TEST);     // Turn off Z-buffer

    projectionMatrix_skybox = projectionMatrix;

    cameraMatrix_skybox = cameraMatrix;
    cameraMatrix_skybox.m[3] = 0;
    cameraMatrix_skybox.m[7] = 0;
    cameraMatrix_skybox.m[11] = 0;

    glUniformMatrix4fv(glGetUniformLocation(program_skybox, "projMatrix"), 1, GL_TRUE, projectionMatrix_skybox.m);
    glUniformMatrix4fv(glGetUniformLocation(program_skybox, "camMatrix"), 1, GL_TRUE, cameraMatrix_skybox.m);
    glUniformMatrix4fv(glGetUniformLocation(program_skybox, "identityMatrix"), 1, GL_TRUE, identityMatrix.m);

    glActiveTexture(GL_TEXTURE0); // Just make sure the texture unit match

    for (i = 0; i < 6; i++)
    {
        glBindTexture(GL_TEXTURE_2D, skyboxTex[i].texID);
        DrawModel(skybox_side[i], program_skybox, "inPosition", NULL, "inTexCoord");
    }


    glEnable(GL_DEPTH_TEST);        // Turn on Z-buffer

    // Binding again just to be sure (for the day the code is mixed with others)
    //glActiveTexture(GL_TEXTURE0); // Just make sure the texture unit match
    //glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);


}
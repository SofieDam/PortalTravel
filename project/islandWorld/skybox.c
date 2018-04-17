// Select texture set by setting this constant to 0 or 6
#define TEXTURE_OFFSET 0

Model *skybox_side[6];

GLuint program_skybox;

// Skybox
GLuint cubemap;
TextureData skyboxTex[6];

mat4 cameraMatrix, projectionMatrix;;

    GLfloat skybox_vertices[6][6*3] =
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

    GLfloat skybox_texcoord[6][6*2] =
            {
                    {
                            1.0, 1.0,
                            1.0, 0.0, // left OK
                            0.0, 0.0,
                            0.0, 1.0,
                    },
                    {
                            0.0, 1.0, // right OK
                            1.0, 1.0,
                            1.0, 0.0,
                            0.0, 0.0,
                    },
                    {
                            1.0, 0.0, // top OK
                            0.0, 0.0,
                            0.0, 1.0,
                            1.0, 1.0,
                    },
                    {
                            0.0, 1.0,
                            1.0, 1.0,
                            1.0, 0.0, // bottom
                            0.0, 0.0,
                    },
                    {
                            0.0, 1.0,
                            1.0, 1.0,
                            1.0, 0.0, // back OK
                            0.0, 0.0,
                    },
                    {
                            1.0, 1.0,
                            1.0, 0.0, // front OK
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
                    /*
                    "image/skyrender0004.tga",
                    "image/skyrender0001.tga",
                    "image/skyrender0003.tga",
                    "image/skyrender0006.tga",
                    "image/skyrender0002.tga",
                    "image/skyrender0005.tga",
                    */

                    "image/sky-blue.tga",   //left
                    "image/sky-blue.tga",   //right
                    "image/sky-blue.tga",   //top
                    "image/sky-blue.tga",   //botton
                    "image/sky-blue.tga",   //back
                    "image/sky-blue.tga",   //front

            };



void initSkybox()
{

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
        printf("Loading texture data %d done\n", i);
    }

    program_skybox = loadShaders("islandWorld/skybox.vert", "islandWorld/skybox.frag");
    projectionMatrix = frustum(-0.001, 0.001, -0.001, 0.001, 0.002, 100.0);
    glUniformMatrix4fv(glGetUniformLocation(program_skybox, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
    glUniform1i(glGetUniformLocation(program_skybox, "tex"), 0); // Texture unit 0
    //loadSkyboxTextures();
}


void loadSkyboxTextures()
{
    int i;

    glGenTextures(1, &cubemap);			// Generate OpenGL texture IDs
    glActiveTexture(GL_TEXTURE0); // Just make sure the texture unit match

    // Note all operations on GL_TEXTURE_CUBE_MAP, not GL_TEXTURE_2D

    // Load texture data and create ordinary texture objects (for skybox)
    for (i = 0; i < 6; i++)
    {
        printf("Loading texture %s\n", textureFileName[i+TEXTURE_OFFSET]);
        LoadTGATexture(textureFileName[i+TEXTURE_OFFSET], &skyboxTex[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
    // Load to cube map
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, skyboxTex[0].w, skyboxTex[0].h, 0, GL_RGBA, GL_UNSIGNED_BYTE, skyboxTex[0].imageData);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, skyboxTex[1].w, skyboxTex[1].h, 0, GL_RGBA, GL_UNSIGNED_BYTE, skyboxTex[1].imageData);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, skyboxTex[2].w, skyboxTex[2].h, 0, GL_RGBA, GL_UNSIGNED_BYTE, skyboxTex[2].imageData);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, skyboxTex[3].w, skyboxTex[3].h, 0, GL_RGBA, GL_UNSIGNED_BYTE, skyboxTex[3].imageData);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, skyboxTex[4].w, skyboxTex[4].h, 0, GL_RGBA, GL_UNSIGNED_BYTE, skyboxTex[4].imageData);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, skyboxTex[5].w, skyboxTex[5].h, 0, GL_RGBA, GL_UNSIGNED_BYTE, skyboxTex[5].imageData);

    //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

}

void displaySkybox(float *R, float *verticalAngle, float *horizontalAngle, float* horizontalHeadAngle)
{
    int i;

// draw box
    glDisable(GL_DEPTH_TEST);
    glUseProgram(program_skybox);

    cameraMatrix = lookAt(
            (*R) * cos(*verticalAngle) * sin(*horizontalAngle),
            (*R) * sin(*verticalAngle),
            (*R) * cos(*verticalAngle) * cos(*horizontalAngle),

            0, (*horizontalHeadAngle), 0,

            0, 1, 0);
    //cameraMatrix = lookAt((*zoom)*sin(*angle),(*zoom)*(*height),(*zoom)*cos(*angle), 0,0,0, 0,1,0);
    cameraMatrix.m[3] = 0;
    cameraMatrix.m[7] = 0;
    cameraMatrix.m[11] = 0;
    glUniformMatrix4fv(glGetUniformLocation(program_skybox, "camMatrix"), 1, GL_TRUE, cameraMatrix.m);

    for (i = 0; i < 6; i++)
    {
        glBindTexture(GL_TEXTURE_2D, skyboxTex[i].texID);
        DrawModel(skybox_side[i], program_skybox, "inPosition", NULL, "inTexCoord");
    }
    glEnable(GL_DEPTH_TEST);

// Binding again just to be sure (for the day the code is mixed with others)
    glActiveTexture(GL_TEXTURE0); // Just make sure the texture unit match
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);


}
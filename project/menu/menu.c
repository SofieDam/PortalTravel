/*
 * Create and draw the main menu and the portal menus for the different worlds.
 * Controls mouse positions and take action if menu item is selected.
 */

// Program
GLuint program_menu;

// Model
Model *square_model;

// Textures
GLuint tex_button_island, tex_button_forest, tex_button_exit, tex_button_background;
GLuint tex_portal_island, tex_portal_forest;
GLuint tex_open_portal, tex_portal_i, tex_portal_f;

// Matrices for model transformation and scaling
mat4 modelMatrix_button_island, modelMatrix_button_forest, modelMatrix_button_exit, modelMatrix_button_background;
mat4  modelMatrix_portal_menu;
mat4  modelMatrix_open_portal, modelMatrix_portal;

int s_size, wrld;

// Mouse over
int mouseOver_i, mouseOver_f, mouseOver_e = 0;
int mouseOver_portal = 0;

// Mouse event
float radius, c_x, c_island_y, c_forest_y, c_exit_y;
float radius_portal, c_portal_x, c_portal_y;

// Portal animation
int portalAnimation = 0;        // {0,1,2}
int portalCounter = 0;          // decides how long the first animation picture will be drawn
float scalePortal = 0.0001;


// For menu objects
GLfloat menu_vertices[4*3] =
        {
                -1.0, -1.0, 0.0,
                1.0, -1.0, 0.0,
                1.0, 1.0, 0.0,
                -1.0, 1.0, 0.0
        };
GLfloat menu_texcoord[4*2] =
        {
                0.0, 1.0,
                1.0, 1.0,
                1.0, 0.0,
                0.0, 0.0,
        };
GLuint  menu_indices[6] = {0, 1, 3, 1, 3, 2};


void getWorld(int *w)
{
    *w = wrld;
}

/*
 * Detects when the mouse position is over one of the menu items.
 * The menu item will then be drawn differently.
 */
void mouseOverListener(int x, int y)
{
    if (wrld == 0)
    {
        x = (float)x;
        y = (float)y;

        float d_island = sqrt(((x - c_x)*(x - c_x)) + ((y - c_island_y)*(y - c_island_y)));
        float d_forest = sqrt(((x - c_x)*(x - c_x)) + ((y - c_forest_y)*(y - c_forest_y)));
        float d_exit = sqrt(((x - c_x)*(x - c_x)) + ((y - c_exit_y)*(y - c_exit_y)));

        if (d_island <= radius)
        {
            mouseOver_i = 1;
            mouseOver_f = 0;
            mouseOver_e = 0;
        }
        else if (d_forest <= radius)
        {
            mouseOver_i = 0;
            mouseOver_f = 1;
            mouseOver_e = 0;
        }
        else if (d_exit <= radius)
        {
            mouseOver_i = 0;
            mouseOver_f = 0;
            mouseOver_e = 1;
        }
        else
        {
            mouseOver_i = 0;
            mouseOver_f = 0;
            mouseOver_e = 0;
        }
    }
    if ((wrld == 1) || (wrld == 2))
    {
        x = (float)x;
        y = (float)y;

        float d = sqrt(((x - c_portal_x)*(x - c_portal_x)) + ((y - c_portal_y)*(y - c_portal_y)));

        if (d <= radius_portal)
        {
            mouseOver_portal = 1;
        }
        else
        {
            mouseOver_portal = 0;
        }
    }
}

/*
 * Detect mouse clicks.
 * Decides which action to take when an menu item is chosen.
 */
void mouseEvent(int button, int state, int x, int y)
{
    // Main menu
    if ((wrld == 0) && (button == 0) && (state == 1))
    {
        //printf("Button: %i, state: %i, x: %i, y: %i\n", button, state, x, y);

        x = (float)x;
        y = (float)y;

        float d_island = sqrt(((x - c_x)*(x - c_x)) + ((y - c_island_y)*(y - c_island_y)));
        float d_forest = sqrt(((x - c_x)*(x - c_x)) + ((y - c_forest_y)*(y - c_forest_y)));
        float d_exit = sqrt(((x - c_x)*(x - c_x)) + ((y - c_exit_y)*(y - c_exit_y)));

        if (d_island <= radius)
        {
            wrld = 1;
        }
        if (d_forest <= radius)
        {
            wrld = 2;
        }
        if (d_exit <= radius)
        {
            exit(0);
        }
    }

    // Portal menu for forest world
    if (((wrld == 1) || (wrld == 2)) && (button == 0) && (state == 1))
    {
        x = (float)x;
        y = (float)y;

        float d = sqrt(((x - c_portal_x)*(x - c_portal_x)) + ((y - c_portal_y)*(y - c_portal_y)));

        // Open portal to world 1
        if (d <= radius_portal)
        {
            portalAnimation = 1;
            portalCounter = 2;
        }
    }

    //printf("Button: %i, state: %i, x: %i, y: %i\n", button, state, x, y);
}




void initMenu(int s, int w)
{
    // Screen size
    s_size = s;

    // World
    wrld = w;


    program_menu = loadShaders("menu/menu.vert", "menu/menu.frag");

    glUniform1i(glGetUniformLocation(program_menu, "tex"), 0); // Texture unit 0
    LoadTGATextureSimple("image/i_world.tga", &tex_button_island);
    LoadTGATextureSimple("image/portal_i_menu.tga", &tex_portal_island);
    LoadTGATextureSimple("image/portal_f_menu.tga", &tex_portal_forest);
    LoadTGATextureSimple("image/f_world.tga", &tex_button_forest);
    LoadTGATextureSimple("image/exit.tga", &tex_button_exit);
    LoadTGATextureSimple("image/background.tga", &tex_button_background);
    LoadTGATextureSimple("image/open_portal.tga", &tex_open_portal);
    LoadTGATextureSimple("image/portal_i.tga", &tex_portal_i);
    LoadTGATextureSimple("image/portal_f.tga", &tex_portal_f);

    square_model = LoadDataToModel(
            menu_vertices,
            NULL,
            menu_texcoord,
            NULL,
            menu_indices,
            4,
            6);

    modelMatrix_button_background = S(-1.0, -1.0, -1.0);

    mat4 scaleMatrix = S(0.2, 0.2, 1.0);
    modelMatrix_button_island = Mult(T(0.0, 0.5, 0.0), scaleMatrix);
    modelMatrix_button_forest = scaleMatrix;
    modelMatrix_button_exit = Mult(T(0.0, -0.5, 0.0), scaleMatrix);


    // Init portal menu
    modelMatrix_portal_menu = Mult(T(-0.92, 0.92, 0.0), S(0.06, 0.06, 0.06));
    modelMatrix_open_portal = S(1.0, 1.0, 1.0);

    // Initialize values for calculation in mouseEvent() and mouseOverListener()
    // to determine if the mouse position appeared in the circle

    float size = 800;

    // Main menu
    radius = (((float)80) / size) * (float)s_size;
    c_x = (((float)402)/size) * (float)s_size;
    c_island_y = (((float)196)/size) * (float)s_size;
    c_forest_y = (((float)398)/size) * (float)s_size;
    c_exit_y = (((float)593)/size) * (float)s_size;

    // Portal menu
    radius_portal = (((float)24) / size) * (float)s_size;
    c_portal_x = (((float)32)/size) * (float)s_size;
    c_portal_y = (((float)32)/size) * (float)s_size;

}


/*
 * Draw a fast and bright light in the middle of the screen.
 */
void drawOpenPortal(void)
{
    // Enable blending for transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glUniformMatrix4fv(glGetUniformLocation(program_menu, "modelMatrix"), 1, GL_TRUE, modelMatrix_open_portal.m);
    glUniform1i(glGetUniformLocation(program_menu, "mouseOver"), 2);
    glBindTexture(GL_TEXTURE_2D, tex_open_portal);
    DrawModel(square_model, program_menu, "inPosition", NULL, "inTexCoord");

    // Disable blending
    glDisable(GL_BLEND);

    if ((portalAnimation == 1) && (portalCounter > 0)) {
        portalCounter -= 1;
        if (portalCounter <= 0)
        {
            portalAnimation = 2;
            portalCounter = 0;
        }
    }
    else {
        portalAnimation = 0;
        portalCounter = 0;
    }
}

/*
 * Draw a portal in the middle of the screen.
 */
void drawPortal(GLuint tex, int w)
{
    modelMatrix_portal = S(scalePortal, scalePortal, scalePortal);

    glUniformMatrix4fv(glGetUniformLocation(program_menu, "modelMatrix"), 1, GL_TRUE, modelMatrix_portal.m);
    glUniform1i(glGetUniformLocation(program_menu, "mouseOver"), 2);
    glBindTexture(GL_TEXTURE_2D, tex);
    DrawModel(square_model, program_menu, "inPosition", NULL, "inTexCoord");

    // Portal animation done
    if (scalePortal > 1.5) {
        portalAnimation = 0;
        scalePortal = 0.0001;
        wrld = w;
    }
        // Increase the size of the portal
    else
    {
        scalePortal += 0.01;
    }
}

/*
 * Draw a menu in the left corner of the screen.
 */
void drawPortalMenu(GLuint tex)
{
    // Enable blending for transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glUniformMatrix4fv(glGetUniformLocation(program_menu, "modelMatrix"), 1, GL_TRUE, modelMatrix_portal_menu.m);
    glUniform1i(glGetUniformLocation(program_menu, "mouseOver"), mouseOver_portal);
    glBindTexture(GL_TEXTURE_2D, tex);
    DrawModel(square_model, program_menu, "inPosition", NULL, "inTexCoord");

    // Disable blending
    glDisable(GL_BLEND);
}


void displayPortalMenu_forestWorld(void)
{
    glUseProgram(program_menu);
    glActiveTexture(GL_TEXTURE0);

    if (portalAnimation == 1)
    {
        drawOpenPortal();
    }
    else if (portalAnimation == 2)
    {
        drawPortal(tex_portal_f, 2);
    }
    else
    {
        drawPortalMenu(tex_portal_forest);
    }
}

void displayPortalMenu_islandWorld(void)
{
    glUseProgram(program_menu);
    glActiveTexture(GL_TEXTURE0);

    if (portalAnimation == 1)
    {
        drawOpenPortal();
    }
    else if (portalAnimation == 2) {

        drawPortal(tex_portal_i, 1);
    }
    else
    {
        drawPortalMenu(tex_portal_island);
    }
}

void displayMenu(void)
{

    glUseProgram(program_menu);

    // Background
    glDisable(GL_DEPTH_TEST);
    glUniformMatrix4fv(glGetUniformLocation(program_menu, "modelMatrix"), 1, GL_TRUE, modelMatrix_button_background.m);
    glUniform1i(glGetUniformLocation(program_menu, "mouseOver"), 2);
    glBindTexture(GL_TEXTURE_2D, tex_button_background);
    DrawModel(square_model, program_menu, "inPosition", NULL, "inTexCoord");
    glEnable(GL_DEPTH_TEST);

    // Enable blending for transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Island World button
    glUniformMatrix4fv(glGetUniformLocation(program_menu, "modelMatrix"), 1, GL_TRUE, modelMatrix_button_island.m);
    glUniform1i(glGetUniformLocation(program_menu, "mouseOver"), mouseOver_i);
    glBindTexture(GL_TEXTURE_2D, tex_button_island);
    DrawModel(square_model, program_menu, "inPosition", NULL, "inTexCoord");

    // Forest World button
    glUniformMatrix4fv(glGetUniformLocation(program_menu, "modelMatrix"), 1, GL_TRUE, modelMatrix_button_forest.m);
    glUniform1i(glGetUniformLocation(program_menu, "mouseOver"), mouseOver_f);
    glBindTexture(GL_TEXTURE_2D, tex_button_forest);
    DrawModel(square_model, program_menu, "inPosition", NULL, "inTexCoord");

    // Exit button
    glUniformMatrix4fv(glGetUniformLocation(program_menu, "modelMatrix"), 1, GL_TRUE, modelMatrix_button_exit.m);
    glUniform1i(glGetUniformLocation(program_menu, "mouseOver"), mouseOver_e);
    glBindTexture(GL_TEXTURE_2D, tex_button_exit);
    DrawModel(square_model, program_menu, "inPosition", NULL, "inTexCoord");

    // Disable blending
    glDisable(GL_BLEND);
}
/*
 * Write the pressed key.
 */
void keyPressed(unsigned char c, int x, int y)
{
    switch (c) {
        case 27:
            exit(0);
            break;
        case GLUT_KEY_UP:
            printf("%s\n", "up");
            break;
        case GLUT_KEY_DOWN:
            printf("%s\n", "down");
            break;
        case GLUT_KEY_RIGHT:
            printf("%s\n", "right");
            break;
        case GLUT_KEY_LEFT:
            printf("%s\n", "left");
            break;
        default:
            printf("%c\n", c);
    }
}

/*
 * Change movement directions from keyboard input.
 */
void keyboard(float *height, float *angle, float *zoom)
{
    if (glutKeyIsDown(27))
    {
        exit(0);
        printf("case 27\n");
    }

    if (glutKeyIsDown(GLUT_KEY_UP))
    {
        *height += 0.1;
    }
    if (glutKeyIsDown(GLUT_KEY_DOWN))
    {
        *height -= 0.1;
    }
    if (glutKeyIsDown(GLUT_KEY_RIGHT))
    {
        *angle += 0.1;
    }
    if (glutKeyIsDown(GLUT_KEY_LEFT))
    {
        *angle -= 0.1;
    }
    if (glutKeyIsDown('z'))
    {
        *zoom -= 0.5;
    }
    if (glutKeyIsDown('x'))
    {
        *zoom += 0.5;
    }

}
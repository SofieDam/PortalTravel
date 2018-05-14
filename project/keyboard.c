#include <stdio.h>
#include <stdlib.h>

/*
 * Write the pressed key.
 */
void keyPressed(unsigned char c, int x, int y)
{
    switch (c) {
            /*
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
             */
    }
}

/*
 * Change movement directions from keyboard input.
 */
void keyboard(float *R, float *verticalAngle, float *horizontalAngle, float* horizontalHeadAngle)
{
    if (glutKeyIsDown(27))
    {
        exit(0);
    }
    if (glutKeyIsDown(GLUT_KEY_UP))
    {
        if (*verticalAngle < (M_PI/2 - 0.1)) {
            *verticalAngle += 0.01;
        }
    }
    if (glutKeyIsDown(GLUT_KEY_DOWN))
    {
        if (*verticalAngle > -(M_PI/2 - 0.1)) {
            *verticalAngle -= 0.01;
        }
    }
    if (glutKeyIsDown(GLUT_KEY_RIGHT))
    {
        *horizontalAngle -= 0.01;
    }
    if (glutKeyIsDown(GLUT_KEY_LEFT))
    {
        *horizontalAngle += 0.01;
    }
    if (glutKeyIsDown('z'))
    {
        *R -= 0.01;
    }
    if (glutKeyIsDown('x'))
    {
        *R += 0.01;
    }
    if (glutKeyIsDown('w')) {
        *horizontalHeadAngle += 0.1;
    };
    if (glutKeyIsDown('s')) {
        *horizontalHeadAngle -= 0.1;
    };

}

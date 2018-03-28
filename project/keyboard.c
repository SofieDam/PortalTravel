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
        *zoom -= 0.1;
    }
    if (glutKeyIsDown('x'))
    {
        *zoom += 0.1;
    }

}











/*
void keyboard(vec3* eye, vec3* center, vec3* up, float speed){

    vec3 forwardDirection = Normalize(VectorSub(*center, *eye));
    vec3 rightDirection = Normalize(CrossProduct(forwardDirection, *up));

    vec3 forwardStep = ScalarMult(forwardDirection, speed);
    vec3 backwardStep = ScalarMult(forwardDirection, -speed);
    vec3 leftStep = ScalarMult(rightDirection, -speed);
    vec3 rightStep = ScalarMult(rightDirection, speed);

    // Move forward
    if (glutKeyIsDown(GLUT_KEY_UP)) {
        *eye = VectorAdd(*eye, forwardStep);
        *center = VectorAdd(*center, forwardStep);
    };

    // Move backward
    if (glutKeyIsDown(GLUT_KEY_DOWN)) {
        *eye = VectorAdd(*eye, backwardStep);
        *center = VectorAdd(*center, backwardStep);
    };

    // Move left
    if (glutKeyIsDown(GLUT_KEY_LEFT)) {
        *eye = VectorAdd(*eye, leftStep);
        *center = VectorAdd(*center, leftStep);
    };

    // Move right
    if (glutKeyIsDown(GLUT_KEY_RIGHT)) {
        *eye = VectorAdd(*eye, rightStep);
        *center = VectorAdd(*center, rightStep);
    };


    // Rotate up
    if (glutKeyIsDown('w')) {
        *center = VectorAdd(*center, ScalarMult(*up, speed));
    };
    // Rotate down
    if (glutKeyIsDown('s')) {
        *center = VectorAdd(*center, ScalarMult(*up, -speed));
    };
    // Rotate left
    if (glutKeyIsDown('a')) {
        *center = VectorAdd(*center, ScalarMult(rightDirection, -speed));
    };
    // rotate right
    if (glutKeyIsDown('d')) {
        *center = VectorAdd(*center, ScalarMult(rightDirection, speed));
    };

}
 */
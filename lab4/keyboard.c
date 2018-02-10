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
void keyboard(vec3* eye, vec3* center, vec3* up, float speed){

    vec3 forwardDirection = Normalize(VectorSub(*center, *eye));
    vec3 rightDirection = Normalize(CrossProduct(forwardDirection, *up));

    // Move forward
    if (glutKeyIsDown(GLUT_KEY_UP)) {
        *eye = VectorAdd(*eye, ScalarMult(forwardDirection, speed));
        *center = VectorAdd(*center, ScalarMult(forwardDirection, speed));
    };

    // Move backward
    if (glutKeyIsDown(GLUT_KEY_DOWN)) {
        *eye = VectorAdd(*eye, ScalarMult(forwardDirection, -speed));
        *center = VectorAdd(*center, ScalarMult(forwardDirection, -speed));
    };

    // Move left
    if (glutKeyIsDown(GLUT_KEY_LEFT)) {
        *eye = VectorAdd(*eye, ScalarMult(rightDirection, -speed));
        *center = VectorAdd(*center, ScalarMult(rightDirection, -speed));
    };

    // Move right
    if (glutKeyIsDown(GLUT_KEY_RIGHT)) {
        *eye = VectorAdd(*eye, ScalarMult(rightDirection, speed));
        *center = VectorAdd(*center, ScalarMult(rightDirection, speed));
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
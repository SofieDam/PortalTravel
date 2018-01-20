# Answers to questions in lab 1

## 1) Setup and getting acquainted with the lab shell

### Where is the origin placed in the on-screen coordinate system?

Bottom left corner.

See: https://learnopengl.com/#!Getting-started/Coordinate-Systems

### Which direction are the X and Y axes pointing in the on-screen coordinate system?

- x: right
- y: up

See: https://learnopengl.com/#!Getting-started/Coordinate-Systems


### The triangle color is controlled from the fragment shader. Would it be possible to control it from the main program? How?
Yes! By passing a matrix of colors to the shader program.

## 2) Transformations in the vertex shader

### What is the purpose of the "in", "out" and "uniform" modifiers?
Modifiers declare how variables are meant to be used.

- The “uniform” modifier is uniform for all vertices. It is information defined per primitive.
- The “in” modifier is an attribute variable, which means that it is information defined per vertex. Attributes can be different for different vertices within a primitive while a uniform must be the same throughout a primitive.
- The “out” modifier  is assigned indices (since it in this lab is assigned a fragment shader output).  The fragment shader output's index corresponds to a draw buffer.

### What is the output of the vertex shader?

The vertex shader passes through the transformation.
The output is an transformation by matrix multiplication which will in our case result in a rotation.

### What does the function glUniformMatrix4fv do?
The function passes a matrix, of 4 vectors, to a shader program.
The function specify the value of a uniform variable for the current program object.

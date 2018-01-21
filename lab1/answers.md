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
- The “in” modifier is an attribute variable, which means that it is information defined per vertex.
Attributes can be different for different vertices within a primitive while a uniform must be the same throughout a primitive.
- The "out" variable will be passed on to the next stage in the pipeline.
The shader must set these variables during some point in its execution.

### What is the output of the vertex shader?
The vertex shader passes through the transformation.
The output in this case is an transformation by matrix multiplication which will result in a rotation.

### What does the function glUniformMatrix4fv do?
The function specify the value of a uniform variable for the current program object.
The function passes a matrix, of 4 vectors with datatype float, to a shader program.


## 3) Simple animation

###  What is the frame rate of the animation?
The frame is updated every 20 ms.
Every time the display is updated, the triangle is rotating with 0.05 rad.

## 4) Color shading

### Did you need to do anything different when uploading the color data?
I needed to create a matrix of colors for every vertex in the triangle.
I also needed to create a VBO (vertex buffer object) for the color matrix, and load it to the shader program and connect it to an in variable in the vertex shader.

### The "in" and "out" modifiers are now used for something different. What?
??

### What is this kind of shading called? What could we use otherwise?
- Gouraud shading (shades are interpolated between vertices).
- We could have used flat shading (same shading for the whole objects) as we did at the beginning of the lab.
We could also have used Phong shading (the normal vector of the vertices is interpolated).

## 5) Building a cube, visible surface detection

### What problems did you encounter while building the cube?
- How to rotate the cube, so that all sides are shown.

### How do you change the facing of a polygon?
By rotating around the x, y and z axis.


## 6) Load a 3D model from disc

### Why do we need normal vectors for a model?
Instead of calculating shading values per vertex, and interpolating them to each pixel,
the normal vector of the vertices is interpolated,
so lighting can be calculated for each pixel with a unique normal vector.
Like before, each vertex has a surface normal,
but this time the surface normals, and not the shades, are interpolated.
Note that the normal vectors can not be used after linear interpolation alone, because if we do,
the length of the vectors will be shorter in the middle.
Thus, normalization is necessary in the fragment shader.

In order to calculate light, normal vectors must be included.

### What did you do in your fragment shader?
Calculate light (by using dot product), and interpolate normal vectors.

### Should a normal vector always be perpendicular to a certain triangle? If not, why?
Yes?

### Now we are using glBindBuffer and glBufferData again. They deal with buffers, but in what way?
They handle different kinds of buffers; vertices, normals and indexes. 
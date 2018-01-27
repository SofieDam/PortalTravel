# Answers to questions in lab 2

## 1) Procedural texture mapping

### How are the textures coordinates mapped on the bunny? Can you see what geometry was used?

- The file "bunnyplus.obj" contains "vt" coordinates, which are the texture coordinates of the vertices. 
These texture coordinates are loaded to the shader program by using VBO.
The coordinates are passed to the fragment shader, where the x (s) and y (t) values are used to create sin-function-like colors. 
- In other words, the texture coordinates are passed as attribute array to the fragment shader.
The texture coordinates are used in a texture generating function in the fragment shader.
- Geometry? Mapping 2D to 3D? Linear mapping?

### What kind of procedural texture did you make?

-  A procedural texture generated inside a fragment shader. 
The fragment value is a function of s and t, in this case a sinus-function of each, mapped onto the red and the green color channels.
- Followed the example of Ingemar’s psychedelic teapot. 

## 2) Texture mapping

### Can we modify how we access the texture? How?
Depending of the kind of texture you use, GLSL has a number of types for texture access such as sampler1D, sampler2D, sampler3D, samplerCube, sampler1DShadow, and sampler2DShadow.
All these refer to textures by the number of their texture units. They are read-only (uniform) and are written by and communicated from the host program. 
We used the type sampler2D in this lab, which is used for reading from plain 2D textures.

### Why can't we just pass the texture object to the shader? There is a specific reason for this, a limited resource. What?
To provide cheaper and more effective memory management. 

## 3) Projection

### How did you move the bunny to get it in view?
- At the beginning, the bunny was placed outside the window coordinates.
But you could see a part of it in the upper part of the screen.
- To get the bunny in view, we placed the Y-position in the origin (zero).

## 4) Viewing using the lookat function

### Given a certain vector for v, is there some place you can't place the camera?
v controls the up-coordinate, which axis/axes/directions the view/camera should rotate around. 
- Not in 0,0,0.
- Not if you make a camera placement outside the screen coordinates. 
With other words, not if you place the camera away from the object (where the object can't be reached).

## 5) Diffuse shading

### Did you implement your light calculations in the vertex or fragment shader? So, which kind of shading did you implement?
- In the fragment shader.
- Phong shading with a specular part.

### Some geometry data must be vec4, others are just as well vec3's. Which ones, and why? How about vertices, light source, normal vectors...?
- The normal matrix, surface matrix, light, reflected light direction and eye direction must be of vec3.
Why? To use linear algebraic operations?
- The gl_position and out_color must be of vec4.
gl_position needs x, y, z positions + clipping.
The clipping part plays its part while linear algebra transformations are applied to the position.
It is set to 1.0 by default.
The out_color needs R,G,B values + transparency. 

## 7) Building a scene + camera movement

### If you rotate an object or rotate the camera, what matrices are affected?
- Object: The rotation matrix.
- Camera: The lookAt matrix. 
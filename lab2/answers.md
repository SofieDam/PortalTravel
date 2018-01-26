# Answers to questions in lab 2

## 1) Procedural texture mapping

### How are the textures coordinates mapped on the bunny? Can you see what geometry was used?

- The file "bunnyplus.obj" contains some "vt", which are the texture coordinates of the vertices. 

### What kind of procedural texture did you make?

- Se sida 136 i boken, finns ett exempel om "Procedural texture". 

## 2) Texture mapping

### Can we modify how we access the texture? How?

### Why can't we just pass the texture object to the shader? There is a specific reason for this, a limited resource. What?

## 3) Projection

### How did you move the bunny to get it in view?
- At the beginning, the bunny was placed outside the window coordinates.
But you could see a part of it in the upper part of the screen.
- To get the bunny in view, we didn't do any translation on the Y-axis (kept it zero).

## 4) Viewing using the lookat function

### Given a certain vector for v, is there some place you can't place the camera?

- Not in 0,0,0.
- Not if make a camera placement outside the screen coordinates.

## 5) Diffuse shading

### Did you implement your light calculations in the vertex or fragment shader? So, which kind of shading did you implement?

### Some geometry data must be vec4, others are just as well vec3's. Which ones, and why? How about vertices, light source, normal vectors...?

## 6) Extra: Gouraud vs Phong

### Was the difference big? If not, why?

## 7) Building a scene + camera movement

### If you rotate an object or rotate the camera, what matrices are affected?
# Answers to questions in lab 3

## 1) Hierarcical modelling, the windmill

### How can you get all four blades to rotate with just one time-dependent rotation matrix?
- Draw 4 blade objects, by starting with one and rotate 90 degrees for every new blade.
- All 4 blades are then updated/rotated and redrawn every frame update.

### How do you make the wings follow the body's movements?
The wings uses the same movements as the body + some extra rotation.

### You may have to tweak some numbers to make the placement right. How can you do that without making a lot of small changes in numbers in the code followed by recompilations?
We set the placements once for the parent.
All its children uses the same placement.
So to change the placement for all objects, you only need to change the placement once (for the parent).

## 2) Manual viewing controls

### What kind of control did you implement?
Keyboard. Following controls have been implemented:
 - Up arrow - move the camera upwards
 - Down arrow - move the camera downwards
 - Left arrow - rotate the camera to the left around the object
 - Right arrow - rotate the camera to the right around the object
 - "z" - zoom in
 - "x" - zoom out
 - "esc" - exit/close the window

### Can you make this kind of control in some other way than manipulating a "look-at" matrix?

## 3) Virtual world and skybox

### How did you handle the camera matrix for the skybox?
By zeroing the translation of the camera matrix.
This will center the skybox around the camera.

### How did you represent the objects? Is this a good way to manage a scene or would you do it differently for a "real" application?
- The ground is represented as a cube. It's rescaled, so that it looks like its flat (2D). 
It is drawn like the skybox...... 

### What special considerations are needed when rendering a skybox?
- Draw without Z-buffering
- Center the skybox around the camera by zeroing the translation of the world-to-view
  matrix.
- No light on the skybox, just texture.
- Filter with GL_CLAMP_TO_EDGE to avoid edge artifacts (we do not think this is necessary in this lab)

### What is the problem with the skybox object used in the lab? (The problem doesn't have to be corrected.)
The skybox has no visible ground and the ground we implement have no "infinite" length.
The result to this is that the background will be shown in the edges of the ground object, since the ground do not reach the edges. 

## 4) Specular shading, external light sources

### How do you generate a vector from the surface to the eye?
By taking the eye position (camera position) minus the surface position. 

```c
eyeDirection = normalize(eyePosition-ex_Surface);
```

### Which vectors need renormalization in the fragment shader?
- View/eye direction
- Positional vectors
- Normal direction (but do this in the vertex shader)

## 5) Multitexturing

### How did you choose to combine the texture colour and the lighting colour?
By multiplication. 

```c
out_Color = vec4(shade, 1.0) * texture(texUnit2, ex_Tex_Coord) * texture(texUnit, ex_Tex_Coord);
```

### How did you choose to combine the two textures?
By multiplication. 
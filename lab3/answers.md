# Answers to questions in lab 3

## 1) Hierarcical modelling, the windmill

### How can you get all four blades to rotate with just one time-dependent rotation matrix?
All objects (including all 4 blades) in the hierarchy are updated and redrawn in every frame update.

### How do you make the wings follow the body's movements?
The wings uses the same movements as the body + some extra rotation.

### You may have to tweak some numbers to make the placement right. How can you do that without making a lot of small changes in numbers in the code followed by recompilations?
We set the placements once for the parent.
All its children uses the same placement.
So to change the placement for all objects, you only need to change the placement once (for the parent).

## 2) Manual viewing controls

### What kind of control did you implement?

### Can you make this kind of control in some other way than manipulating a "look-at" matrix?

## 3) Virtual world and skybox

### How did you handle the camera matrix for the skybox?

### How did you represent the objects? Is this a good way to manage a scene or would you do it differently for a "real" application?

### What special considerations are needed when rendering a skybox?

### What is the problem with the skybox object used in the lab? (The problem doesn't have to be corrected.)

## 4) Specular shading, external light sources

### How do you generate a vector from the surface to the eye?

### Which vectors need renormalization in the fragment shader?

## 5) Multitexturing

### How did you choose to combine the texture colour and the lighting colour?

### How did you choose to combine the two textures?
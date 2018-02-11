# Answers to questions in lab 4

## 1) Load and inspect the heightmap.

### What kind of data does the heightmap image data hold? What range is the heightmap data?
- Data: grayscale values.
- Range: 0-255.

### The terrain is 4x4. How many polygons does it draw?
18 triangles/polygons.

## 2) Navigating the heightmap 

### Did you need to extend your camera movement code? How?
Yes. 
We added straight movements to the right and to the left.   

## 3) Calculate normal vectors and apply lighting

### Which method did you use to find the normal vector?
The second one. 
Good, fast approximation. 
Pick three neighbor vertices, on all sides of the vertex, and use these three for calculating a normal vector.

## 4) Calculating map height for a point

### How did you figure out what surface to use? 

### How did you calculate the height from the surface?

## 5) Pick one of the "extras" below as final part of the lab. 
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
We figured out which quad the point belonged to. 
Then we calculated whether it was in the upper or lower triangle of the quad. 

### How did you calculate the height from the surface?
By using plane equation.
For upper triangle:
```c
float height = v1 + xDiff*(v2 - v1) + zDiff*(v3 - v1);
```
For lower triangle:
```c
float height = v4 + (1.0-xDiff)*(v2 - v4) + (1.0-zDiff)*(v3 - v4);
```

## 5) Pick one of the "extras" below as final part of the lab. 

### 5b)
#### How did you handle collisions? 
By checking the distance to all other objetcs and the terrain edges. 
We changed the direction to the opposite if an collision was detected.
    
#### How did you represent the objects to make collision detection easy/managable?
By representing them in an global array, containing a struct for each object.
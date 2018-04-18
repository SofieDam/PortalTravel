/*
 * Variables needed to build a cube.
 */

#include "VectorUtils3.h"


const int cube_sides = 6;
const float cube_width = 2.0;

// First vector (lower left corner) to generate a cube side
const static vec3 cube_vertices[6] = {
        {-1.0, -1.0, 1.0},            // front
        {1.0, -1.0, 1.0},             // right
        {1.0, -1.0, -1.0},            // back
        {-1.0, -1.0, -1.0},           // left
        {-1.0, 1.0, 1.0},            // up
        {1.0, -1.0, 1.0},            // bottom
};

// XYZ-transformation for each cube side
const static mat3 cube_xyz_plane[6] = {
        // x    y    z      x    y    z      x    y    z
        {{1.0, 0.0, 0.0,   0.0, 1.0, 0.0,   0.0, 0.0, 1.0}},     // front
        {{0.0, 0.0, 1.0,   0.0, 1.0, 0.0,   -1.0, 0.0, 0.0}},     //right
        {{-1.0, 0.0, 0.0,   0.0, 1.0, 0.0,   0.0, 0.0, -1.0}},    // back
        {{0.0, 0.0, -1.0,   0.0, 1.0, 0.0,   1.0, 0.0, 0.0}},     // left
        {{1.0, 0.0, 0.0,   0.0, 0.0, 1.0,   0.0, -1.0, 0.0}},    // up
        {{-1.0, 0.0, 0.0,   0.0, 0.0, -1.0,   0.0, -1.0, 0.0}},   // bottom
};



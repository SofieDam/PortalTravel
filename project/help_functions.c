#include "VectorUtils3.h"

// Calculates normal vector for triangle
vec3 calculateNormalVector(vec3 nv1, vec3 nv2, vec3 nv3){
    vec3 v1 = VectorSub(nv2, nv1);
    vec3 v2 = VectorSub(nv3, nv1);
    vec3 normalVector =  Normalize(CrossProduct(v1, v2));
    return normalVector;
}

// Calculate indices
void calculateIndices(GLuint *indexArray, int w, int i, int r, int c) {
    // Triangle 1
    indexArray[(r + (c*(w-1)) + (i*(w-1)*(w-1)))*6 + 0] = r + (c*w) + (i*w*w);
    indexArray[(r + (c*(w-1)) + (i*(w-1)*(w-1)))*6 + 1] = (r+1) + (c*w) + (i*w*w);
    indexArray[(r + (c*(w-1)) + (i*(w-1)*(w-1)))*6 + 2] = r + ((c+1)*w) + (i*w*w);
    // Triangle 2
    indexArray[(r + (c*(w-1)) + (i*(w-1)*(w-1)))*6 + 3] = (r+1) + (c*w) + (i*w*w);
    indexArray[(r + (c*(w-1)) + (i*(w-1)*(w-1)))*6 + 4] = r + ((c+1)*w) + (i*w*w);
    indexArray[(r + (c*(w-1)) + (i*(w-1)*(w-1)))*6 + 5] = (r+1) + ((c+1)*w) + (i*w*w);
}

// Calculate texture coordinates
void calculateTextures(GLfloat *texCoordArray, int w, int i, int r, int c) {
    texCoordArray[(r + (c*w) + (i*w*w))*2 + 0] = -r;
    texCoordArray[(r + (c*w) + (i*w*w))*2 + 1] = -c;
}

// Calculate normal vectors.
// Pick three neighbor vertices, on all sides of the vertex,
// and use these three for calculating a normal vector.
void calculateNormals(GLfloat *vertexArray, GLfloat *normalArray, int w, int i, int r, int c) {

    vec3 nv1, nv2, nv3;    // Neighboring vertices

    nv1.x = vertexArray[(r + (c*w) + (i*w*w))*3 + 0];
    nv1.y = vertexArray[(r + (c*w) + (i*w*w))*3 + 1];
    nv1.z = vertexArray[(r + (c*w) + (i*w*w))*3 + 2];

    nv2.x = vertexArray[((r+1) + (c*w) + (i*w*w))*3 + 0];
    nv2.y = vertexArray[((r+1) + (c*w) + (i*w*w))*3 + 1];
    nv2.z = vertexArray[((r+1) + (c*w) + (i*w*w))*3 + 2];

    nv3.x = vertexArray[(r + ((c+1)*w) + (i*w*w))*3 + 0];
    nv3.y = vertexArray[(r + ((c+1)*w) + (i*w*w))*3 + 1];
    nv3.z = vertexArray[(r + ((c+1)*w) + (i*w*w))*3 + 2];

    vec3 normalVector =  calculateNormalVector(nv1, nv2, nv3);

    normalArray[(r + (c*w) + (i*w*w))*3 + 0] = normalVector.x;
    normalArray[(r + (c*w) + (i*w*w))*3 + 1] = normalVector.y;
    normalArray[(r + (c*w) + (i*w*w))*3 + 2] = normalVector.z;
}

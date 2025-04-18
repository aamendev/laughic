#ifndef VECTOR_3D_H
#define VECTOR_3D_H
#include <stdio.h>
typedef struct Vector_3d
{
    float x;
    float y;
    float z;
} Vector3d;
float dot(Vector3d* v1, Vector3d* v2);
Vector3d cross(Vector3d* v1, Vector3d* v2);
Vector3d project(Vector3d* v1, Vector3d* v2);
Vector3d reject(Vector3d* v1, Vector3d* v2);
Vector3d add(Vector3d* v1, Vector3d* v2);
Vector3d sub(Vector3d* v1, Vector3d* v2);
float get_theta(Vector3d* v1, Vector3d* v2);
Vector3d normalise(Vector3d* v);
Vector3d scale(Vector3d* v, float factor);
float squared_magnitude(Vector3d* v1);
float magnitude(Vector3d* v1);
void print_vector(Vector3d* v);
#endif

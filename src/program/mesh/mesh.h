#ifndef __MESH_H__
#define __MESH_H__

#include <SDL2/SDL.h>
#include "../linalg/linalg.h"

#define NUM_TRIANGLE_POINTS 3
#define COORD_3D_DIMS 3
#define VECTOR_4D_SIZE 4

typedef struct Triangle
{
    Vector** points;
    Vector* normal;
    double shade;
} Triangle;

typedef struct Mesh
{
    int num_triangles;
    Vector* rotation_angles;
    Triangle** triangles;
} Mesh;

Triangle* create_triangle();
void destroy_triangle(Triangle* t);
void arr_to_triangle(Triangle* t, double *arr);
Mesh* create_mesh(int num_triangles);
void destroy_mesh(Mesh *m);
void arr_to_mesh(Mesh* m, double** arr);
double** to_dynamic(int x, int y, double arr[x][y]);
void free2Darr(int x, int y, double** dynamic_arr);
void translate_mesh(Mesh* m_in, Vector* trans, Mesh* m_out);
void rotate_mesh(Mesh* mesh, Vector* orientation_vec);
void copy_mesh(Mesh* m1, Mesh* m2);
void copy_triangle(Triangle* t1, Triangle* t2);
void print_mesh(Mesh* mesh);
void generate_rotatation_mat(Matrix *rotation_mat, double angles[COORD_3D_DIMS]);

#endif
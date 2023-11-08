#include "mesh.h"
#include <stdlib.h>

Triangle* create_triangle();
void destroy_triangle(Triangle* t);
void arr_to_triangle(Triangle* t, double *arr);
Mesh* create_mesh(int num_triangles);
void destroy_mesh(Mesh *m);
void arr_to_mesh(Mesh* m, double** arr);
double** to_dynamic(int x, int y, double arr[x][y]);
void free2Darr(int x, int y, double** dynamic_arr);
void get_normal(Triangle* t, Vector* normal);

Triangle* create_triangle()
{
    Triangle* t = (Triangle*)malloc(sizeof(Triangle));
    t->points = (Vector**)malloc(NUM_TRIANGLE_POINTS * sizeof(Vector*));
    for (int i = 0; i < NUM_TRIANGLE_POINTS; i++)
    {
        t->points[i] = create_vec(VECTOR_4D_SIZE);
    }

    t->normal = create_vec(COORD_3D_DIMS);
    t->shade = 1;
    return t;
}

void destroy_triangle(Triangle* t)
{
    for (int i = 0; i < NUM_TRIANGLE_POINTS; i++)
    {
        destroy_vec(t->points[i]);
    }

    free(t->points);
    free(t);
}

void arr_to_triangle(Triangle* t, double *arr)
{
    for (int i = 0; i < NUM_TRIANGLE_POINTS; i++)
    {
        arr_to_vec(t->points[i], arr + (i * COORD_3D_DIMS), COORD_3D_DIMS);
    }
    get_normal(t, t->normal);
}

Mesh* create_mesh(int num_triangles)
{
    Mesh* m = (Mesh*)malloc(sizeof(Mesh));
    m->num_triangles = num_triangles;
    m->triangles = (Triangle**)malloc(m->num_triangles * sizeof(Triangle*));
    for (int i = 0; i < m->num_triangles; i++)
    {
        m->triangles[i] = create_triangle();
    }

    m->rotation_angles = create_vec(COORD_3D_DIMS);
    zero_vec(m->rotation_angles);

    return m;
}

void destroy_mesh(Mesh *m)
{
    for (int i = 0; i < m->num_triangles; i++)
    {
        destroy_triangle(m->triangles[i]);
    }
    destroy_vec(m->rotation_angles);

    free(m->triangles);
    free(m);
}

void arr_to_mesh(Mesh* m, double** arr)
{
    for (int i = 0; i < m->num_triangles; i++)
    {
        arr_to_triangle(m->triangles[i], arr[i]);
    }
}

double** to_dynamic(int x, int y, double arr[x][y])
{
    double** dynamic_arr = (double**)malloc(x * sizeof(double*));
    for (int i = 0; i < x; i++)
    {
        dynamic_arr[i] = (double*)malloc(y * sizeof(double));
        for (int j = 0; j < y; j++)
        {
            dynamic_arr[i][j] = arr[i][j];
        }
    }
    return dynamic_arr;
}

void free2Darr(int x, int y, double** dynamic_arr)
{
    for (int i = 0; i < x; i++)
    {
        free(dynamic_arr[i]);
    }
    free(dynamic_arr);
}

void translate_triangle(Triangle *in_t, Vector* trans, Triangle* out_t)
{
    for (int j = 0; j < NUM_TRIANGLE_POINTS; j++ )
        {
            vec_add(in_t->points[j], trans, out_t->points[j]);
        }

    //set_normal(out_t);
}

void translate_mesh(Mesh* m_in, Vector* trans, Mesh* m_out)
{
    for (int i = 0; i < m_in->num_triangles; i++)
    {
        translate_triangle(m_in->triangles[i], trans, m_out->triangles[i]);
    }
}

void print_mesh(Mesh* mesh) 
{
    for (int i = 0; i < mesh->num_triangles; i++)
    {
        for (int j = 0; j < NUM_TRIANGLE_POINTS; j++ )
        {
            print_vec(mesh->triangles[i]->points[j]);
            printf(",\n");
        }
    }
}

void get_normal(Triangle* t, Vector* normal)
{
    Vector* line1 = create_vec(COORD_3D_DIMS);
    vec_sub(t->points[1], t->points[0], line1);

    Vector* line2 = create_vec(COORD_3D_DIMS);
    vec_sub(t->points[2], t->points[0], line2);

    cross_product(line1, line2, normal);
    vec_normalize(normal, normal);

    destroy_vec(line1); destroy_vec(line2);
}

void transform_triangle(Triangle* t_in, Matrix* transformation, Triangle* t_out)
{
    Vector* tmp_in = create_vec(VECTOR_4D_SIZE);
    Vector* tmp_out = create_vec(VECTOR_4D_SIZE);

    for (int j = 0; j < NUM_TRIANGLE_POINTS; j++)
    {
        copy_vec(t_in->points[j], tmp_in);
        mat_vec_mul(transformation, tmp_in, tmp_out);
        copy_vec(tmp_out, t_out->points[j]);
    }
    get_normal(t_out, t_out->normal);

    destroy_vec(tmp_in); destroy_vec(tmp_out);
}

void transform_mesh(Mesh* m_in, Matrix* transform, Mesh* m_out)
{
    for (int i = 0; i < m_in->num_triangles; i++)
    {
        for (int j = 0; j < NUM_TRIANGLE_POINTS; j++)
        {
            transform_triangle(m_in->triangles[i], transform, m_out->triangles[i]);
        }
    }
}

void generate_rotatation_mat(Matrix *rotation_mat, double angles[COORD_3D_DIMS])
{
    double sin_alpha = sin(angles[X_AXIS]); double cos_alpha = cos(angles[X_AXIS]);
    double sin_beta = sin(angles[Y_AXIS]); double cos_beta= cos(angles[Y_AXIS]);
    double sin_gamma = sin(angles[Z_AXIS]); double cos_gamma = cos(angles[Z_AXIS]);

    double rot_arr[VECTOR_4D_SIZE][VECTOR_4D_SIZE] = {
        { cos_beta*cos_gamma, (sin_alpha*sin_beta*cos_gamma) - (cos_alpha*sin_gamma), cos_alpha*sin_beta*cos_gamma + sin_alpha*sin_gamma, 0}, 
        { cos_beta*sin_gamma, sin_alpha*sin_beta*sin_gamma + cos_alpha*cos_gamma, cos_alpha*sin_beta*sin_gamma - sin_alpha*cos_gamma, 0}, 
        { -sin_beta, sin_alpha*cos_beta, cos_alpha*cos_beta, 0},
        { 0, 0, 0, 1}
    };
    
    arr_to_mat(rotation_mat, &rot_arr[0][0]);
}

void rotate_mesh(Mesh* mesh, Vector* orientation_vec)
{
    double angles[COORD_3D_DIMS];
    for (int i = 0; i < COORD_3D_DIMS; i++)
    {
        angles[i] = index_vec(orientation_vec, i) - index_vec(mesh->rotation_angles, i);
    }
    Matrix* rotation_mat = create_mat(VECTOR_4D_SIZE, VECTOR_4D_SIZE);
    generate_rotatation_mat(rotation_mat, angles);
    transform_mesh(mesh, rotation_mat, mesh);

    copy_vec(orientation_vec, mesh->rotation_angles);
    destroy_mat(rotation_mat);
}

void copy_mesh(Mesh* m1, Mesh* m2)
{
    if (m1->num_triangles == m2->num_triangles)
    {
        for (int i = 0; i < m1->num_triangles; i++)
        {
            copy_triangle(m1->triangles[i], m2->triangles[i]);
        }
    }
    else 
    {
        printf("Meshes are of different size.\n");
        return;
    }

    copy_vec(m1->rotation_angles, m2->rotation_angles);

}

void copy_triangle(Triangle* t1, Triangle* t2)
{
    for (int i = 0; i < NUM_TRIANGLE_POINTS; i++)
    {
        copy_vec(t1->points[i], t2->points[i]);
    }
    copy_vec(t1->normal, t2->normal);
    t2->shade = t1->shade;
}
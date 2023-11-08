#ifndef __LINALG_H__
#define __LINALG_H__

#include <stdio.h>

#define PI 3.1415926
#define X_AXIS 0
#define Y_AXIS 1
#define Z_AXIS 2

typedef struct Vector
{
    int rows;
    double* data;
} Vector;

typedef struct Matrix
{
    int rows, cols;
    double* data;
} Matrix;

Vector* create_vec(int rows);
void destroy_vec(Vector* v);
Matrix* create_mat(int rows, int cols);
void destroy_mat(Matrix* A);
double index_mat(Matrix* A, int i, int j);
double index_vec(Vector* v, int i);
void arr_to_vec(Vector* v, double* arr, int arr_size);
void arr_to_mat(Matrix* A, double* arr);
void print_mat(Matrix* A);
void print_vec(Vector* v);
double get_vec_length(Vector* v);
void vec_add(Vector* v, Vector* u, Vector* k);
void vec_sub(Vector* v, Vector* u, Vector* k);
void mat_add(Matrix* A, Matrix* B, Matrix* C);
void mat_sub(Matrix* A, Matrix* B, Matrix* C);
void mat_vec_mul(Matrix* A, Vector* v, Vector* k);
void mat_mul(Matrix* A, Matrix* B, Matrix* C);
void vec_div(Vector* v, double num, Vector* k);
void vec_mul_num(Vector* v, double num, Vector* k);
void copy_vec(Vector* v, Vector* k);
Vector* extend_vector_by_one(Vector* v);
void zero_vec(Vector* v);
double handle_rads(double angle);
void cross_product(Vector* v, Vector* u, Vector* k);
double dot_product(Vector* v, Vector* u);
void vec_normalize(Vector* v, Vector* k);
void vec_mat_mul(Matrix* A, Vector* v, Vector* k);

#endif
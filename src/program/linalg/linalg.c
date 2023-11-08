#include "linalg.h"
#include <stdlib.h>
#include <math.h>

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

Vector* create_vec(int rows)
{
    Vector* v = (Vector*)malloc(sizeof(Vector));
    if (v)
    {
        v->rows = rows;
        v->data = (double*)malloc(rows * sizeof(double));
        if (v->data)
        {
            for (int i = 0; i < rows; i++)
            {
                v->data[i] = 1;
            }
            return v;
        }
        else
        {
            printf("Vector data allocation failed.\n");
            return NULL;
        }
    }
    else
    {
        printf("Vector allocation failed.\n");
        return NULL;
    }
}

void destroy_vec(Vector* v)
{
    free(v->data);
    free(v);
}

Matrix* create_mat(int rows, int cols)
{
    Matrix* A = (Matrix*)malloc(sizeof(Matrix));
    if (A)
    {
        A->rows = rows; A->cols = cols;
        A->data = (double*)malloc((rows * cols) * sizeof(double));
        if (A->data)
        {
            for (int i = 0; i < rows; i++)
            {
                for (int j = 0; j < cols; j++)
                {
                    A->data[(i * cols) + j] = 0;
                }
            }

            return A;
        }
        else
        {
            printf("Matrix data allocation failed.\n");
            return NULL;
        }
    }
    else
    {
        printf("Matrix allocation failed.\n");
        return NULL;
    }
}

void destroy_mat(Matrix* A)
{
    free(A->data);
    free(A);
}

double index_mat(Matrix* A, int i, int j)
{
    int rows = A->rows; int cols = A->cols;
    if (i < rows && i >= 0 && j < cols && j >= 0)
    {
        return A->data[(i * cols) + j];
    }
    else
    {
        printf("Index out of bounds.\n");
        return -1;
    }
    
}

double index_vec(Vector* v, int i)
{
    int rows = v->rows;
    if (i < rows && i >= 0)
    {
        return v->data[i];
    }
    else
    {
        printf("Index out of bounds.\n");
        return -1;
    }
    
}

void zero_vec(Vector* v)
{
    for (int i = 0; i < v->rows; i++)
    {
        v->data[i] = 0;
    }
}

void arr_to_vec(Vector* v, double* arr, int arr_size)
{
    int len = v->rows;
    if (arr_size < v->rows)
    {
        len = arr_size;
    }
    for (int i = 0; i < len; i++)
    {
        v->data[i] = arr[i];
    }
}

void arr_to_mat(Matrix* A, double* arr)
{
    int rows = A->rows; int cols = A->cols;
    for (int i = 0; i < A->rows; i++)
    {
        for (int j = 0; j < A->cols; j++)
        {
            A->data[(i * cols) + j] = arr[(i * cols) + j];
        }
    }
}

void print_mat(Matrix* A)
{
    for (int i = 0; i < A->rows; i++)
    {
        for (int j = 0; j < A->cols; j++)
        {
            printf("%lf ", index_mat(A, i, j));
        }
        printf("\n");
    }
}

void print_vec(Vector* v)
{
    for (int i = 0; i < v->rows; i++)
    {
        printf("%lf ", index_vec(v, i));
    }
    printf("   ");
}

double get_vec_length(Vector* v)
{
    int rows = v->rows;
    double sum = 0;
    for (int i = 0; i < rows; i++)
    {
        sum += pow(index_vec(v, i), 2);
    }
    return sqrt(sum);
}

int min(int a, int b)
{
    if (a > b) {
        return b;
    }
    else return a;
}
void vec_add(Vector* v, Vector* u, Vector* k)
{
    int rows = min(v->rows, u->rows);
    rows = min(rows, k->rows);
    for (int i = 0; i < rows; i++)
    {
        k->data[i] = index_vec(v, i) + index_vec(u, i);
    }

}

void vec_sub(Vector* v, Vector* u, Vector* k)
{
    int rows = min(v->rows, u->rows);
    rows = min(rows, k->rows);
    for (int i = 0; i < rows; i++)
    {
        k->data[i] = index_vec(v, i) - index_vec(u, i);
    }
}

void vec_div(Vector* v, double num, Vector* k)
{
    int rows = v->rows;
    if (k->rows != rows)
    {
        printf("Vectors are of different sizes.\n");
        return;
    }
    else if (num == 0)
    {
        //printf("Division by 0.\n");
        return;
    }
    else
    {
        for (int i = 0; i < rows; i++)
        {
            k->data[i] = index_vec(v, i) / num;
        }
    }
}

void vec_mul_num(Vector* v, double num, Vector* k)
{
    int rows = v->rows;
    if (k->rows != rows)
    {
        printf("Vectors are of different sizes.\n");
        return;
    }
    else
    {
        for (int i = 0; i < rows; i++)
        {
            k->data[i] = index_vec(v, i) * num;
        }
    }
}

void mat_add(Matrix* A, Matrix* B, Matrix* C)
{
    int rows = A->rows; int cols = A->cols;
    if ((B->rows == rows && C->rows == rows) && (B->cols == cols && C->cols == cols))
    {
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                C->data[(i * cols) + j] = index_mat(A, i, j) + index_mat(B, i, j);
            }
        }
    }
    else
    {
        printf("Matrices are of different sizes.\n");
    }
}

void mat_sub(Matrix* A, Matrix* B, Matrix* C)
{
    int rows = A->rows; int cols = A->cols;
    if ((B->rows == rows && C->rows == rows) && (B->cols == cols && C->cols == cols))
    {
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                C->data[(i * cols) + j] = index_mat(A, i, j) - index_mat(B, i, j);
            }
        }
    }
    else
    {
        printf("Matrices are of different sizes.\n");
    }
}

void mat_vec_mul(Matrix* A, Vector* v, Vector* k)
{
    int mat_rows = A->rows; int mat_cols = A->cols;
    int vec_rows = v->rows;

    if ((vec_rows == mat_cols) && (k->rows == mat_rows))
    {
        for (int i = 0; i < mat_rows; i++)
        {
            double value = 0;
            for (int j = 0; j < mat_cols; j++)
            {
                value += index_mat(A, i, j) * index_vec(v, j);
            }
            k->data[i] = value;
        }
    }
    else
    {
        printf("Matrices are of different sizes.\n");
    }
}

void vec_mat_mul(Matrix* A, Vector* v, Vector* k)
{
    int mat_rows = A->rows; int mat_cols = A->cols;
    int vec_rows = v->rows;

    if ((vec_rows == mat_cols) && (k->rows == mat_rows))
    {
        for (int i = 0; i < mat_cols; i++)
        {
            double value = 0;
            for (int j = 0; j < mat_rows; j++)
            {
                value += index_mat(A, j, i) * index_vec(v, j);
            }
            k->data[i] = value;
        }
    }
    else
    {
        printf("Matrices are of different sizes.\n");
    }
}

void mat_mul(Matrix* A, Matrix* B, Matrix* C)
{
    int inner_rows = B->rows; int outer_rows = A->rows;
    int inner_cols = A->cols; int outer_cols = B->cols;
    if ((inner_rows == inner_cols) && (C->rows == outer_rows && C->cols == outer_cols))
    {
        for (int i = 0; i < outer_rows; i++)
        {
            for (int j = 0; j < outer_cols; j++)
            {
                double value = 0;
                for (int k = 0; k < inner_rows; k++)
                {
                    value += index_mat(A, i, k) * index_mat(B, k, j);
                }
                C->data[(i * outer_cols) + j] = value;
            }
        }
    }
    else
    {
        printf("Matrices are of different sizes.\n");
    }
}

void copy_vec(Vector* v, Vector* k)
{
    int rows = v->rows;
    if (k->rows < rows)
    {
        rows = k->rows;
    }

    for (int i = 0; i < rows; i++)
    {
        k->data[i] = index_vec(v, i);
    }
}

Vector* extend_vector_by_one(Vector* v)
{
    Vector* extended_v = create_vec(v->rows + 1);
    for (int i = 0; i < v->rows; i++)
    {
        extended_v->data[i] = index_vec(v, i);
    }
    extended_v->data[v->rows] = 1;

    return extended_v;
}

void cross_product(Vector* v, Vector* u, Vector* k)
{
    k->data[0] = v->data[1] * u->data[2] - v->data[2] * u->data[1];
    k->data[1] = v->data[2] * u->data[0] - v->data[0] * u->data[2];
    k->data[2] = v->data[0] * u->data[1] - v->data[1] * u->data[0];
}

double dot_product(Vector* v, Vector* u)
{
    double product = 0;
    for (int i = 0; i < v->rows; i++)
    {
        product += index_vec(v, i) * index_vec(u, i);
    }
    return product;
}

void vec_normalize(Vector* v, Vector* k)
{
    vec_div(v, get_vec_length(v), k);
}

double handle_rads(double angle)
{
    if (angle >= 2*PI) angle = 0;
    if (angle < 0) angle = 2*PI;

    return angle;
}
/*
#define ROWS 3
#define COLS 4

int main()
{
    Matrix* A = create_mat(ROWS, COLS);
    double arr1[ROWS][COLS] = { { 1, 2, 3, 4 }, { 5, 6, 7, 8 }, { 9, 10, 11, 12 } };
    double* p1 = &arr1[0][0];
    arr_to_mat(A, p1);
    printf("Matrix A:\n");
    print_mat(A);

    Matrix* B = create_mat(ROWS, COLS);
    arr_to_mat(B, p1);
    printf("Matrix B:\n");
    print_mat(B);

    Matrix* C = create_mat(ROWS, COLS);
    mat_add(A, B, C);
    printf("Matrix C:\n");
    print_mat(C);

    
    Vector* v = create_vec(COLS);
    double arr2[COLS] = { 1, 2, 3, 4 };
    double* p2 = &arr2[0];
    arr_to_vec(v, p2);
    printf("Vector v length: %lf\n", get_vec_length(v));
    
    Vector* k = create_vec(ROWS);
    mat_vec_mul(A, v, k);
    printf("Vector k:\n");
    print_vec(k);
    
}
*/
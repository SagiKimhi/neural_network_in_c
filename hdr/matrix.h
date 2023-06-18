#ifndef MATRIX_H_
#define MATRIX_H_

/*
 * Includes:
 * --------- */
#include <stddef.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

/*
 * Macros:
 * ------- */
#ifndef NN_MALLOC
    #include <stdlib.h>
    #define NN_MALLOC malloc
#endif /*NN_MALLOC*/

#ifndef NN_ASSERT
    #include <assert.h>
    #define NN_ASSERT assert
#endif /*NN_ASSERT*/

#define MATRIX_AT(m, row, col)\
    (m).data[(row) * (m).cols + (col)]

/* 
 * Typedefs:
 * --------- */
typedef struct {
    size_t rows;
    size_t cols;
    float *data;
} matrix_t;


/* 
 * Function Declarations:
 * ---------------------- */  
/* Allocation Methods */
extern matrix_t matrix_alloc(size_t num_rows, size_t num_cols);
extern void matrix_free(matrix_t m);

/* Mathematic Methods */
extern void matrix_dot(matrix_t dst, matrix_t a, matrix_t b);
extern void matrix_sum(matrix_t dst, matrix_t m);

/* Utility Methods */
extern void matrix_rand(matrix_t m, float low, float high);
extern void matrix_print(matrix_t m);

/* Internal Use Methods */
extern float rand_float(void);


#endif /*MATRIX_H_*/


#ifdef MATRIX_IMPLEMENTATION_

/*
 * Allocation Methods:
 * ------------------- */
matrix_t matrix_alloc(size_t num_rows, size_t num_cols)
{
    matrix_t m;

    m.rows  = num_rows;
    m.cols  = num_cols;
    m.data  = malloc(sizeof(*m.data) * num_rows * num_cols);
    NN_ASSERT(m.data);
    return m;
}

void matrix_free(matrix_t m)
{
    free(m.data);
}

/*
 * Mathematic Methods:
 * ------------------- */
void matrix_dot(matrix_t dst, matrix_t a, matrix_t b)
{
    (void) dst;
    (void) a;
    (void) b;
}

void matrix_sum(matrix_t dst, matrix_t m)
{
    (void) dst;
    (void) m;
}

/*
 * Utility Methods:
 * ---------------- */
void matrix_rand(matrix_t m, float low, float high)
{
    NN_ASSERT(m.data);
    NN_ASSERT(low <= high);

    for (size_t row = 0; row < m.rows; row++) {
        for (size_t col = 0; col < m.cols; col++) {
            MATRIX_AT(m, row, col) = ( 
                rand_float() * (high - low) + low
            );
        }
    }
}

void matrix_print(matrix_t m)
{
    NN_ASSERT(m.data);

    for (size_t row = 0; row < m.rows; row++) {
        for (size_t col = 0; col < m.cols; col++) {
            printf("%f ", MATRIX_AT(m, row, col));
        }
        printf("\n");
    }
}

/*
 * Internal Methods:
 * ----------------- */
float rand_float(void)
{
    return (
        ((float) rand()) / ((float)RAND_MAX)
    );
}

#endif /*MATRIX_IMPLEMENTATION_*/

#ifndef NN_MATRIX_H_
#define NN_MATRIX_H_

/* ---------
 * Includes:
 * --------- */
#include <nn_common_includes.h>
#include <nn_rand.h>

/* --------
 * Defines:
 * -------- */
#define NN_MATRIX_AT(m, row, col)\
    (m).data[(row) * (m).cols + (col)]


/* ---------
 * Typedefs:
 * --------- */
typedef struct {
    size_t rows;
    size_t cols;
    float *data;
} nn_matrix_t;


/* ----------------------
 * Function Declarations:
 * ---------------------- */  

/* Allocation Methods */
extern nn_matrix_t nn_matrix_alloc(size_t num_rows, size_t num_cols);
extern void nn_matrix_free(nn_matrix_t m);

/* Value Setters */
extern void nn_matrix_memset(nn_matrix_t m, float val);
extern void nn_matrix_rand(nn_matrix_t m, float low, float high);

/* Mathematic Methods */
extern void nn_matrix_dot(nn_matrix_t dst, nn_matrix_t a, nn_matrix_t b);
extern void nn_matrix_sum(nn_matrix_t dst, nn_matrix_t m);

/* Utility Methods */
extern void nn_matrix_print(nn_matrix_t m);


#endif /*NN_MATRIX_H_*/


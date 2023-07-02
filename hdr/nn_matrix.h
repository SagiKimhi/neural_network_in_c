#ifndef NN_MATRIX_H_
#define NN_MATRIX_H_

/* ---------
 * Includes:
 * --------- */
#include <nn_common_includes.h>
#include <nn_rand.h>
#include <nn_math.h>

/* --------
 * Defines:
 * -------- */
#define NN_MATRIX_AT(m, row, col)\
    (m).data[(row) * (m).stride + (col)]

#define NN_MATRIX_PRINT(m) nn_matrix_print(m, #m, 0)


/* ---------
 * Typedefs:
 * --------- */
typedef struct {
    size_t rows;
    size_t cols;
    size_t stride;
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
extern void nn_matrix_sigmoid(nn_matrix_t m);
extern void nn_matrix_actf(nn_matrix_t m, nn_act_func_enum act_func);

/* Utility Methods */
extern void nn_matrix_copy(nn_matrix_t dst, nn_matrix_t src);
extern nn_matrix_t nn_matrix_row(nn_matrix_t m, size_t row);
extern void nn_matrix_print(nn_matrix_t m, char *name, int indent);
extern void nn_matrix_fprint(FILE *stream, nn_matrix_t m, const char *name, int indent);
extern void nn_matrix_save(FILE *fp,nn_matrix_t m);
extern nn_matrix_t nn_matrix_load(FILE *fp);


#endif /*NN_MATRIX_H_*/



#ifndef NN_MATRIX_C_
#define NN_MATRIX_C_

#include "nn_defines.h"
#include <nn_matrix.h>

/* -------------------
 * Allocation Methods:
 * ------------------- */
nn_matrix_t nn_matrix_alloc(size_t num_rows, size_t num_cols)
{
    nn_matrix_t m;

    m.rows      = num_rows;
    m.cols      = num_cols;
    m.stride    = num_cols;
    m.data      = NN_MALLOC(sizeof(*m.data) * num_rows * num_cols);

    NN_ASSERT(m.data);

    nn_matrix_memset(m, 0);

    return m;
}

void nn_matrix_free(nn_matrix_t m)
{
    NN_ASSERT(m.data);
    NN_FREE(m.data);
}


/* --------------
 * Value Setters:
 * -------------- */
void nn_matrix_memset(nn_matrix_t m, float val)
{
    NN_ASSERT(m.data);

    for (size_t row = 0; row < m.rows; row++)
        for (size_t col = 0; col < m.cols; col++)
            NN_MATRIX_AT(m, row, col) = val;
}

void nn_matrix_rand(nn_matrix_t m, float low, float high)
{
    NN_ASSERT(m.data);
    NN_ASSERT(low <= high);

    for (size_t row = 0; row < m.rows; row++) {
        for (size_t col = 0; col < m.cols; col++) {
            NN_MATRIX_AT(m, row, col) = ( 
                rand_float() * (high - low) + low
            );
        }
    }
}


/* -----------------------------
 * Mathematic Operation Methods:
 * ----------------------------- */
void nn_matrix_dot(nn_matrix_t dst, nn_matrix_t a, nn_matrix_t b)
{
    NN_ASSERT(a.cols == b.rows);
    NN_ASSERT(dst.rows == a.rows);
    NN_ASSERT(dst.cols == b.cols);
    NN_ASSERT(a.data && b.data && dst.data);

    size_t n = a.cols;

    nn_matrix_memset(dst, 0);

    for (size_t i = 0; i < dst.rows; i++) {
        for (size_t j = 0; j < dst.cols; j++) {
            for (size_t k = 0; k < n; k++) {
                NN_MATRIX_AT(dst, i, j) += (
                        NN_MATRIX_AT(a, i, k) * NN_MATRIX_AT(b, k, j)
                );
            }
        }
    }
}

void nn_matrix_sum(nn_matrix_t dst, nn_matrix_t m)
{
    NN_ASSERT(m.rows == dst.rows);
    NN_ASSERT(m.cols == dst.cols);
    NN_ASSERT(m.data && dst.data);
    
    for (size_t row = 0; row < dst.rows; row++) {
        for (size_t col = 0; col < dst.cols; col++) {
            NN_MATRIX_AT(dst, row, col) += NN_MATRIX_AT(m, row, col);
        }
    }
}

void nn_matrix_sigmoid(nn_matrix_t m)
{
    NN_ASSERT(m.data);

    for (size_t row = 0; row < m.rows; row++) {
        for (size_t col = 0; col < m.cols; col++) {
            NN_MATRIX_AT(m, row, col) = sigmoidf(NN_MATRIX_AT(m, row, col));
        }
    }
}

void nn_matrix_actf(nn_matrix_t m, nn_act_func_enum act_func)
{
    NN_ASSERT(m.data);

    for (size_t row = 0; row < m.rows; row++)
        for (size_t col = 0; col < m.cols; col++)
            NN_MATRIX_AT(m, row, col) = 
                actf(NN_MATRIX_AT(m, row, col), act_func);
}

/* ----------------
 * Utility Methods:
 * ---------------- */
void nn_matrix_copy(nn_matrix_t dst, nn_matrix_t src)
{
    NN_ASSERT(dst.rows == src.rows);
    NN_ASSERT(dst.cols == src.cols);
    NN_ASSERT(dst.data && src.data);

    for (size_t row = 0; row < dst.rows; row++)
        for (size_t col = 0; col < dst.cols; col++)
            NN_MATRIX_AT(dst, row, col) = NN_MATRIX_AT(src, row, col);
}

nn_matrix_t nn_matrix_row(nn_matrix_t m, size_t row)
{
    return (nn_matrix_t) {
        .rows   = 1,
        .cols   = m.cols,
        .stride = m.stride,
        .data   = &NN_MATRIX_AT(m, row, 0),
    };
}

void nn_matrix_print(nn_matrix_t m, char *name, int indent)
{
    NN_ASSERT(m.data);

    if (name)
        printf("%*s%s = ", indent, "", name);
    
    printf("{\n");

    for (size_t row = 0; row < m.rows; row++) {
        printf("%*s\t[ ", indent, "");

        for (size_t col = 0; col < m.cols; col++) {
            printf(
                "%f%s ", 
                NN_MATRIX_AT(m, row, col), (col + 1 < m.cols ? ",": "")
            );
        }

        printf("]%s\n", (row + 1 < m.rows ? ",": ""));
    }

    printf("%*s}\n", indent, "");
}


#endif /* NN_MATRIX_C_ */

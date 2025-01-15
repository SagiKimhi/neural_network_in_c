#ifndef NN_MATRIX_H_
#define NN_MATRIX_H_

/* ---------
 * Includes:
 * --------- */
#include "nn_common_includes.h"

#ifndef STANDALONE_NN_MATRIX_IMPLEMENTATION

# ifndef NN_RAND_IMPLEMENTATION
#  define NN_RAND_IMPLEMENTATION
#  include "nn_rand.h"
# endif

# ifndef NN_IO_IMPLEMENTATION
#  define NN_IO_IMPLEMENTATION
#  include "nn_io.h"
# endif

# ifndef NN_MATH_IMPLEMENTATION
#  define NN_MATH_IMPLEMENTATION
#  include "nn_math.h"
# endif

#endif /* STANDALONE_NN_MATRIX_IMPLEMENTATION */

/* --------
 * Defines:
 * -------- */
#define NN_MATRIX_AT(m, row, col) (m).data[(row) * (m).stride + (col)]

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
extern nn_matrix_t
nn_matrix_alloc(size_t num_rows, size_t num_cols);
extern void
nn_matrix_free(nn_matrix_t m);

/* Value Setters */
extern void
nn_matrix_memset(nn_matrix_t m, float val);
extern void
nn_matrix_rand(nn_matrix_t m, float low, float high);

/* Mathematic Methods */
extern void
nn_matrix_dot(nn_matrix_t dst, nn_matrix_t a, nn_matrix_t b);
extern void
nn_matrix_sum(nn_matrix_t dst, nn_matrix_t m);
extern void
nn_matrix_sigmoid(nn_matrix_t m);
extern void
nn_matrix_actf(nn_matrix_t m, nn_act_func_enum act_func);

/* Utility Methods */
extern void
nn_matrix_shuffle_rows(nn_matrix_t m);
extern void
nn_matrix_copy(nn_matrix_t dst, nn_matrix_t src);
extern void
nn_matrix_print(nn_matrix_t m, char *name, int indent);
extern void
nn_matrix_fprint(FILE *stream, nn_matrix_t m, const char *name, int indent);
extern void
nn_matrix_save(FILE *fp, nn_matrix_t m);
extern nn_matrix_t
nn_matrix_load(FILE *fp);
extern nn_matrix_t
nn_matrix_row(nn_matrix_t m, size_t row);

#endif /*NN_MATRIX_H_*/

#ifdef NN_MATRIX_IMPLEMENTATION

/* -------------------
 * Allocation Methods:
 * ------------------- */
nn_matrix_t
nn_matrix_alloc(size_t num_rows, size_t num_cols)
{
    nn_matrix_t m;

    m.rows = num_rows;
    m.cols = num_cols;
    m.stride = num_cols;
    m.data = NN_MALLOC(sizeof(*m.data) * num_rows * num_cols);

    NN_ASSERT(m.data);

    nn_matrix_memset(m, 0);

    return m;
}

void
nn_matrix_free(nn_matrix_t m)
{
    NN_ASSERT(m.data);
    NN_FREE(m.data);
}

/* --------------
 * Value Setters:
 * -------------- */
void
nn_matrix_memset(nn_matrix_t m, float val)
{
    NN_ASSERT(m.data);

    for (size_t row = 0; row < m.rows; row++)
        for (size_t col = 0; col < m.cols; col++)
            NN_MATRIX_AT(m, row, col) = val;
}

void
nn_matrix_rand(nn_matrix_t m, float low, float high)
{
    NN_ASSERT(m.data);
    NN_ASSERT(low <= high);

    for (size_t row = 0; row < m.rows; row++) {
        for (size_t col = 0; col < m.cols; col++) {
            NN_MATRIX_AT(m, row, col) = (rand_float() * (high - low) + low);
        }
    }
}

/* -----------------------------
 * Mathematic Operation Methods:
 * ----------------------------- */
void
nn_matrix_dot(nn_matrix_t dst, nn_matrix_t a, nn_matrix_t b)
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
                NN_MATRIX_AT(dst, i, j)
                    += (NN_MATRIX_AT(a, i, k) * NN_MATRIX_AT(b, k, j));
            }
        }
    }
}

void
nn_matrix_sum(nn_matrix_t dst, nn_matrix_t m)
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

void
nn_matrix_sigmoid(nn_matrix_t m)
{
    NN_ASSERT(m.data);

    for (size_t row = 0; row < m.rows; row++) {
        for (size_t col = 0; col < m.cols; col++) {
            NN_MATRIX_AT(m, row, col) = sigmoidf(NN_MATRIX_AT(m, row, col));
        }
    }
}

void
nn_matrix_actf(nn_matrix_t m, nn_act_func_enum act_func)
{
    NN_ASSERT(m.data);

    for (size_t row = 0; row < m.rows; row++)
        for (size_t col = 0; col < m.cols; col++)
            NN_MATRIX_AT(m, row, col)
                = actf(NN_MATRIX_AT(m, row, col), act_func);
}

/* ----------------
 * Utility Methods:
 * ---------------- */
void
nn_matrix_shuffle_rows(nn_matrix_t m)
{
    for (size_t i = 0; i < m.rows; i++) {
        size_t j = rand() % (m.rows - i);

        if (i == j)
            continue;

        for (size_t k = 0; k < m.cols; k++) {
            float tmp = NN_MATRIX_AT(m, i, k);
            NN_MATRIX_AT(m, i, k) = NN_MATRIX_AT(m, j, k);
            NN_MATRIX_AT(m, j, k) = tmp;
        }
    }
}

void
nn_matrix_copy(nn_matrix_t dst, nn_matrix_t src)
{
    NN_ASSERT(dst.rows == src.rows);
    NN_ASSERT(dst.cols == src.cols);
    NN_ASSERT(dst.data && src.data);

    for (size_t row = 0; row < dst.rows; row++)
        for (size_t col = 0; col < dst.cols; col++)
            NN_MATRIX_AT(dst, row, col) = NN_MATRIX_AT(src, row, col);
}

nn_matrix_t
nn_matrix_row(nn_matrix_t m, size_t row)
{
    return (nn_matrix_t){
        .rows = 1,
        .cols = m.cols,
        .stride = m.stride,
        .data = &NN_MATRIX_AT(m, row, 0),
    };
}

void
nn_matrix_print(nn_matrix_t m, char *name, int indent)
{
    nn_matrix_fprint(stdout, m, name, indent);
}

void
nn_matrix_fprint(FILE *stream, nn_matrix_t m, const char *name, int indent)
{
    NN_ASSERT(m.data);

    if (name)
        fprintf(stream, "%*s%s = ", indent, "", name);

    fprintf(stream, "{\n");

    for (size_t row = 0; row < m.rows; row++) {
        fprintf(stream, "%*s\t[ ", indent, "");

        for (size_t col = 0; col < m.cols; col++) {
            fprintf(
                stream,
                "%f%s ",
                NN_MATRIX_AT(m, row, col),
                (col + 1 < m.cols ? "," : ""));
        }

        fprintf(stream, "]%s\n", (row + 1 < m.rows ? "," : ""));
    }

    fprintf(stream, "%*s}\n", indent, "");
}

void
nn_matrix_save(FILE *fp, nn_matrix_t m)
{
    NN_ASSERT(m.data);
    NN_ASSERT(!ferror(fp));

    fwrite(&m.rows, sizeof(m.rows), 1, fp);
    fwrite(&m.cols, sizeof(m.cols), 1, fp);

    for (size_t row = 0; row < m.rows; row++) {
        fwrite(&NN_MATRIX_AT(m, row, 0), sizeof(m.data[0]), m.cols, fp);
    }

    NN_ASSERT(!ferror(fp));
}

nn_matrix_t
nn_matrix_load(FILE *fp)
{
    nn_matrix_t m;

    fread(&m.rows, sizeof(m.rows), 1, fp);
    fread(&m.cols, sizeof(m.cols), 1, fp);

    m = nn_matrix_alloc(m.rows, m.cols);

    fread(m.data, sizeof(m.data[0]), m.rows * m.cols, fp);

    NN_ASSERT(m.data);
    NN_ASSERT(!ferror(fp));

    return m;
}

#endif /* NN_MATRIX_IMPLEMENTATION */

#define NN_MATRIX_IMPLEMENTATION
#include "nn_matrix.h"

typedef struct {
    nn_matrix_t a0;
    nn_matrix_t a1, w1, b1;
    nn_matrix_t a2, w2, b2;
} nn_xor_t;

nn_xor_t nn_xor_alloc(void);
void nn_xor_free(nn_xor_t xor);
void nn_xor_forward(nn_xor_t xor);
float nn_xor_cost(nn_xor_t xor, nn_matrix_t t_in, nn_matrix_t t_out);
void nn_xor_finite_difference(
    nn_xor_t xor, nn_xor_t gradient, float eps, 
    nn_matrix_t t_in, nn_matrix_t t_out
);
void nn_xor_learn(nn_xor_t xor, nn_xor_t gradient, float rate);

#define NN_XOR_PRINT(xor)\
    NN_MATRIX_PRINT(xor.a0);\
    NN_MATRIX_PRINT(xor.w1);\
    NN_MATRIX_PRINT(xor.b1);\
    NN_MATRIX_PRINT(xor.a1);\
    NN_MATRIX_PRINT(xor.w2);\
    NN_MATRIX_PRINT(xor.b2);\
    NN_MATRIX_PRINT(xor.a2)



int main(int argc, char **argv)
{
    srand(time(0));

    float td[] = {
        0, 0, 0,
        0, 1, 1,
        1, 0, 1,
        1, 1, 0,
    };

    nn_xor_t    xor, gradient;
    nn_matrix_t t_in, t_out;
    size_t      n, stride;
    float       eps, rate;

    eps         = 1e-1;
    rate        = 1e-1;
    stride      = 3;
    n           = (sizeof(td) / sizeof(td[0]) / stride);
    xor         = nn_xor_alloc();
    gradient    = nn_xor_alloc();

    t_in = (nn_matrix_t) {
        .rows   = n,
        .cols   = 2,
        .stride = stride,
        .data   = td,
    };

    t_out = (nn_matrix_t) {
        .rows   = n,
        .cols   = 1,
        .stride = stride,
        .data   = &td[2],
    };


    nn_matrix_rand(xor.w1, 0, 1);
    nn_matrix_rand(xor.b1, 0, 1);
    nn_matrix_rand(xor.w2, 0, 1);
    nn_matrix_rand(xor.b2, 0, 1);

    printf("cost before learn = %f\n\n", nn_xor_cost(xor, t_in, t_out));

    for (size_t i = 0; i < 100*1000; i++) {
        nn_xor_finite_difference(xor, gradient, eps, t_in, t_out);
        nn_xor_learn(xor, gradient, rate);

        if (!(i%100))
            printf( "%zu: cost after learn = %f\n", i, nn_xor_cost(xor, t_in, t_out));
    }

    printf("final cost = %f\n\n", nn_xor_cost(xor, t_in, t_out));

#if 1
    for (size_t i = 0; i < 2; i++) {
        for (size_t j = 0; j < 2; j++) {
            NN_MATRIX_AT(xor.a0, 0, 0) = i;
            NN_MATRIX_AT(xor.a0, 0, 1) = j;
            nn_xor_forward(xor);

            printf(
                "%zu ^ %zu = %f\n", 
                i, j, *xor.a2.data
            );
        }
    }
#endif

    nn_xor_free(xor);
    nn_xor_free(gradient);
    
    return 0;
}

nn_xor_t nn_xor_alloc(void)
{
    nn_xor_t xor;

    xor.a0  = nn_matrix_alloc(1, 2);
    xor.w1  = nn_matrix_alloc(2, 2);
    xor.b1  = nn_matrix_alloc(1, 2);
    xor.a1  = nn_matrix_alloc(1, 2);
    xor.w2  = nn_matrix_alloc(2, 1);
    xor.b2  = nn_matrix_alloc(1, 1);
    xor.a2  = nn_matrix_alloc(1, 1);

    return xor;
}

void nn_xor_free(nn_xor_t xor)
{
    nn_matrix_free(xor.a0);
    nn_matrix_free(xor.w1);
    nn_matrix_free(xor.b1);
    nn_matrix_free(xor.a1);
    nn_matrix_free(xor.w2);
    nn_matrix_free(xor.b2);
    nn_matrix_free(xor.a2);
}

void nn_xor_learn(nn_xor_t xor, nn_xor_t gradient, float rate)
{
    for (size_t i = 0; i < xor.w1.rows; i++) {
        for (size_t j = 0; j < xor.w1.cols; j++) {
            NN_MATRIX_AT(xor.w1, i, j) -= (
                rate * NN_MATRIX_AT(gradient.w1, i, j)
            );
        }
    }

    for (size_t i = 0; i < xor.b1.rows; i++) {
        for (size_t j = 0; j < xor.b1.cols; j++) {
            NN_MATRIX_AT(xor.b1, i, j) -= (
                rate * NN_MATRIX_AT(gradient.b1, i, j)
            );
        }
    }

    for (size_t i = 0; i < xor.w2.rows; i++) {
        for (size_t j = 0; j < xor.w2.cols; j++) {
            NN_MATRIX_AT(xor.w2, i, j) -= (
                rate * NN_MATRIX_AT(gradient.w2, i, j)
            );
        }
    }

    for (size_t i = 0; i < xor.b2.rows; i++) {
        for (size_t j = 0; j < xor.b2.cols; j++) {
            NN_MATRIX_AT(xor.b2, i, j) -= (
                rate * NN_MATRIX_AT(gradient.b2, i, j)
            );
        }
    }
}

void nn_xor_finite_difference(
    nn_xor_t xor, nn_xor_t gradient, float eps, 
    nn_matrix_t t_in, nn_matrix_t t_out
)
{
    float saved, original_cost;
    
    original_cost = nn_xor_cost(xor, t_in, t_out);

    for (size_t i = 0; i < xor.w1.rows; i++) {
        for (size_t j = 0; j < xor.w1.cols; j++) {
            saved = NN_MATRIX_AT(xor.w1, i, j);
            NN_MATRIX_AT(xor.w1, i, j) += eps;
            NN_MATRIX_AT(gradient.w1, i, j)    = (
                (nn_xor_cost(xor, t_in, t_out) - original_cost) / eps
            );
            NN_MATRIX_AT(xor.w1, i, j) = saved;
        }
    }

    for (size_t i = 0; i < xor.b1.rows; i++) {
        for (size_t j = 0; j < xor.b1.cols; j++) {
            saved = NN_MATRIX_AT(xor.b1, i, j);
            NN_MATRIX_AT(xor.b1, i, j) += eps;
            NN_MATRIX_AT(gradient.b1, i, j)    = (
                (nn_xor_cost(xor, t_in, t_out) - original_cost) / eps
            );
            NN_MATRIX_AT(xor.b1, i, j) = saved;
        }
    }

    for (size_t i = 0; i < xor.w2.rows; i++) {
        for (size_t j = 0; j < xor.w2.cols; j++) {
            saved = NN_MATRIX_AT(xor.w2, i, j);
            NN_MATRIX_AT(xor.w2, i, j) += eps;
            NN_MATRIX_AT(gradient.w2, i, j)    = (
                (nn_xor_cost(xor, t_in, t_out) - original_cost) / eps
            );
            NN_MATRIX_AT(xor.w2, i, j) = saved;
        }
    }

    for (size_t i = 0; i < xor.b2.rows; i++) {
        for (size_t j = 0; j < xor.b2.cols; j++) {
            saved = NN_MATRIX_AT(xor.b2, i, j);
            NN_MATRIX_AT(xor.b2, i, j) += eps;
            NN_MATRIX_AT(gradient.b2, i, j)    = (
                (nn_xor_cost(xor, t_in, t_out) - original_cost) / eps
            );
            NN_MATRIX_AT(xor.b2, i, j) = saved;
        }
    }
}

float nn_xor_cost(nn_xor_t xor, nn_matrix_t t_in, nn_matrix_t t_out)
{

    float       cost;
    size_t      n_samples;
    nn_matrix_t x, y;

    NN_ASSERT(t_in.rows == t_out.rows);
    NN_ASSERT(t_out.cols == xor.a2.cols);

    cost       = 0;
    n_samples   = t_in.rows;

    for (size_t i = 0; i < n_samples; i++) {
        x   = nn_matrix_row(t_in, i);
        y   = nn_matrix_row(t_out, i);

        nn_matrix_copy(xor.a0, x);
        nn_xor_forward(xor);
        
        for (size_t j = 0; j < t_out.cols; j++) {
            float d = (NN_MATRIX_AT(xor.a2, 0, j) - NN_MATRIX_AT(y, 0, j));
            cost   += (d*d);
        }
    }

    return (cost / n_samples);
}

void nn_xor_forward(nn_xor_t xor)
{
#if 0
    nn_matrix_print(xor.a0, "xor.a0 before forward");
    nn_matrix_print(xor.a1, "xor.a1 before forward");
    nn_matrix_print(xor.a2, "xor.a2 before forward");
#endif

    nn_matrix_dot(xor.a1, xor.a0, xor.w1);
    nn_matrix_sum(xor.a1, xor.b1);
    nn_matrix_sigmoid(xor.a1);

    nn_matrix_dot(xor.a2, xor.a1, xor.w2);
    nn_matrix_sum(xor.a2, xor.b2);
    nn_matrix_sigmoid(xor.a2);

#if 0
    nn_matrix_print(xor.a0, "xor.a0 after forward");
    nn_matrix_print(xor.a1, "xor.a1 after forward");
    nn_matrix_print(xor.a2, "xor.a2 after forward");
#endif
}


#include <nn_tests.h>
#include <stdio.h>

#define STRINGIFY(s) #s

void test_nn_print(void)
{
    size_t arch[] = {2, 2, 1};

    nn_t nn = nn_alloc(arch, NN_SIZEOF_ARR(arch));

    NN_PRINT(nn);
    nn_free(nn);
}

void test_nn_rand(void)
{
    size_t arch[] = {4, 7, 2, 13, 5};

    nn_t nn = nn_alloc(arch, NN_SIZEOF_ARR(arch));

    nn_rand(nn, 0, 10);
    NN_PRINT(nn);
    nn_free(nn);
}

void test_nn_forward(void)
{
    size_t      arch[]  = {2, 2, 1};
    float       td[]    = {
        0, 0, 0,
        0, 1, 1,
        1, 0, 1,
        1, 1, 0,
    };
    size_t      n       = NN_SIZEOF_ARR(td);
    size_t      stride   = 3;
    nn_matrix_t t_in  = {
        .rows   = n,
        .cols   = 2,
        .stride = stride,
        .data   = td,
    };
    nn_t        nn = nn_alloc(arch, NN_SIZEOF_ARR(arch));

    nn_rand(nn, 0, 10);
    nn_matrix_copy(NN_INPUT(nn), nn_matrix_row(t_in, 1));
    NN_MATRIX_PRINT(NN_INPUT(nn));
    nn_forward(nn);
    NN_MATRIX_PRINT(NN_OUTPUT(nn));

    nn_free(nn);
}

void test_nn_with_xor_model(void)
{
    /* Training Set Init */
    float       xor_td[]    = {
        0, 0, 0,
        0, 1, 1,
        1, 0, 1,
        1, 1, 0,
    };
    size_t stride = 3;
    nn_matrix_t ts_in  = {
        .rows   = 4,
        .cols   = 2,
        .stride = stride,
        .data   = xor_td,
    };
    nn_matrix_t ts_out = {
        .rows   = 4,
        .cols   = 1,
        .stride = stride,
        .data   = &xor_td[2],
    };

    /* NN Model init */
    size_t xor_arch[] = {2, 2, 1};
    float rate = 1;
    nn_t nn_xor = nn_alloc(xor_arch, NN_SIZEOF_ARR(xor_arch));
    nn_t nn_grad = nn_alloc(xor_arch, NN_SIZEOF_ARR(xor_arch));

    nn_rand(nn_xor, 0, 1);
    
    printf("\n\nTeaching My AI Model how to XOR :O \n\n");
    printf("-------------------------------------------------------\n\n");
    printf("Cost function value before training: %f\n", nn_cost(nn_xor, ts_in, ts_out));
    printf("Model results before training:\n");
    for (size_t i = 0; i < 2; i++) {
        for (size_t j = 0; j < 2; j++) {
            NN_MATRIX_AT(NN_INPUT(nn_xor), 0, 0) = i;
            NN_MATRIX_AT(NN_INPUT(nn_xor), 0, 1) = j;
            nn_forward(nn_xor);
            printf("%zu ^ %zu = %f\n", i, j, *NN_OUTPUT(nn_xor).data);
        }
    }
    printf("-------------------------------------------------------\n\n");
    printf("Press enter to start the training process...\n");
    char buf[2];
    fgets(buf, 2, stdin);

    for (size_t i = 0; i < 10 * 1000; i++) {
#if 0
        nn_finite_difference(nn_xor, nn_grad, 1e-1, ts_in, ts_out);
#else
        nn_back_propagation(nn_xor, nn_grad, ts_in, ts_out);
#endif
        nn_learn(nn_xor, nn_grad, rate);
        if (!(i%100))
            printf("%zu: cost = %f\n", i, nn_cost(nn_xor, ts_in, ts_out));
    }

    printf("-------------------------------------------------------\n\n");

    printf("Final Cost = %f\n", nn_cost(nn_xor, ts_in, ts_out));
    printf("Model results after training:\n");
    for (size_t i = 0; i < 2; i++) {
        for (size_t j = 0; j < 2; j++) {
            NN_MATRIX_AT(NN_INPUT(nn_xor), 0, 0) = i;
            NN_MATRIX_AT(NN_INPUT(nn_xor), 0, 1) = j;
            nn_forward(nn_xor);
            printf("%zu ^ %zu = %f\n", i, j, *NN_OUTPUT(nn_xor).data);
        }
    }

    nn_free(nn_xor);
    nn_free(nn_grad);
}

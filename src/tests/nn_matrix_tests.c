#ifndef NN_MATRIX_TESTS_C_
#define NN_MATRIX_TESTS_C_

#include "nn_matrix_tests.h"

void test_nn_matrix_alloc(void)
{
    nn_matrix_t a = nn_matrix_alloc(5, 5);

    printf("Testing Matrix Alloc and Memset Function\n");
    NN_MATRIX_PRINT(a);
    printf(" ------------------------------------------------ \n");

    nn_matrix_free(a);
}

void test_nn_matrix_rand(void)
{
    nn_matrix_t m   = nn_matrix_alloc(5, 5);


    printf("Testing Matrix Rand Function (Low = -10 High = 25):\n");
    nn_matrix_print(m, "m_before_rand", 0);
    nn_matrix_rand(m, -10, 25);
    nn_matrix_print(m, "m_after_rand", 0);
    printf(" ------------------------------------------------ \n");

    nn_matrix_free(m);
}

void test_nn_matrix_sum(void)
{
    nn_matrix_t a   = nn_matrix_alloc(10, 10);
    nn_matrix_t b   = nn_matrix_alloc(10, 10);
    nn_matrix_t sum = nn_matrix_alloc(10, 10);


    nn_matrix_memset(a, 1.f);
    nn_matrix_memset(b, 2.f);

    nn_matrix_copy(sum, a);
    nn_matrix_sum(sum, b);

    printf("Testing Matrix Sum Function.\n");
    NN_MATRIX_PRINT(a);
    NN_MATRIX_PRINT(b);
    NN_MATRIX_PRINT(sum);
    printf(" ------------------------------------------------ \n");

    nn_matrix_free(a);
    nn_matrix_free(b);
    nn_matrix_free(sum);
}

void test_nn_matrix_dot(void)
{
    nn_matrix_t a, b, c;

    a   = nn_matrix_alloc(2, 3);
    b   = nn_matrix_alloc(3, 4);
    c   = nn_matrix_alloc(2, 4);

    nn_matrix_memset(a, 3.f);
    nn_matrix_memset(b, 1.5f);
    nn_matrix_dot(c, a, b);

    printf("Testing Matrix Dot Function (A * B = C).\n");
    NN_MATRIX_PRINT(a);
    NN_MATRIX_PRINT(b);
    NN_MATRIX_PRINT(c);
    printf(" ------------------------------------------------ \n");

    nn_matrix_free(a);
    nn_matrix_free(b);
    nn_matrix_free(c);
}

void test_nn_matrix_sigmoid(void)
{
    nn_matrix_t m = nn_matrix_alloc(5, 5);
    
    printf("Testing Matrix Sigmoid Function.\n");
    nn_matrix_rand(m, -15, 25);
    nn_matrix_print(m, "m_before_sigmoid", 0);
    nn_matrix_sigmoid(m);
    nn_matrix_print(m, "m_after_sigmoid", 0);
    printf(" ------------------------------------------------ \n");

    nn_matrix_free(m);
}

#endif /* NN_MATRIX_TESTS_C_ */


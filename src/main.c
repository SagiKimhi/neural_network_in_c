#include <nn_matrix.h>

void test_nn_matrix_alloc(void)
{
    nn_matrix_t a;

    a = nn_matrix_alloc(5, 5);

    printf("Testing Matrix Alloc and Memset Function\n");
    printf("A:\n");
    nn_matrix_print(a);

    nn_matrix_free(a);
}

void test_nn_matrix_rand(void)
{
    nn_matrix_t a;

    a = nn_matrix_alloc(5, 5);

    printf("Testing Matrix Rand Function:\n");
    printf("Matrix Before Rand:\n");
    nn_matrix_print(a);
    printf("Matrix After Rand - Low = 0, High = 30:\n");
    nn_matrix_rand(a, 0, 30);
    nn_matrix_print(a);

    nn_matrix_free(a);
}

void test_nn_matrix_sum(void)
{
    nn_matrix_t a, b;

    a   = nn_matrix_alloc(10, 10);
    b   = nn_matrix_alloc(10, 10);

    nn_matrix_memset(a, 1.f);
    nn_matrix_memset(b, 2.f);

    printf("Testing Matrix Sum Function (A+B).\n");
    printf("A:\n");
    nn_matrix_print(a);
    printf("B:\n");
    nn_matrix_print(b);
    printf("A + B:\n");
    nn_matrix_sum(a, b);
    nn_matrix_print(a);

    nn_matrix_free(a);
    nn_matrix_free(b);
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
    printf("A:\n");
    nn_matrix_print(a);
    printf("B:\n");
    nn_matrix_print(b);
    printf("C:\n");
    nn_matrix_print(c);

    nn_matrix_free(a);
    nn_matrix_free(b);
    nn_matrix_free(c);
}


int main(int arg, char **argv)
{
    srand(time(NULL));

    test_nn_matrix_alloc();
    test_nn_matrix_rand();
    test_nn_matrix_dot();
    test_nn_matrix_sum();
    
    return 0;
}


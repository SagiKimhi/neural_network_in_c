#include <nn_matrix_tests.h>
#include <nn_training_set_tests.h>

void run_matrix_tests(void)
{
    test_nn_matrix_alloc();
    test_nn_matrix_rand();
    test_nn_matrix_sum();
    test_nn_matrix_dot();
    test_nn_matrix_sigmoid();
}

void run_training_set_tests(void)
{
    test_nn_ts_alloc_and_macros();
}

void run_tests(void)
{
    run_matrix_tests();
    run_training_set_tests();
}

int main(int arg, char **argv)
{
#ifdef NN_TESTS
    srand(time(NULL));
    run_tests();
#endif /* NN_TESTS */

    return 0;
}


#include <nn_matrix_tests.h>
#include <nn_training_set_tests.h>
#include <nn_tests.h>

void run_nn_tests(void)
{
    test_nn_with_xor_model();
}

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

void run_framework_tests(void)
{
    run_matrix_tests();
    run_training_set_tests();
}

void run_tests(void)
{
    run_nn_tests();
}

int main(int arg, char **argv)
{
    srand(time(NULL));

#ifdef NN_FW_TESTS
    run_framework_tests();
#endif /* NN_FW_TESTS */

#ifdef NN_TESTS
    run_tests();
#endif /* NN_TESTS */

    return 0;
}


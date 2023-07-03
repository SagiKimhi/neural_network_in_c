#ifndef NN_MATRIX_TESTS_H_
#define NN_MATRIX_TESTS_H_

#include "nn_common_includes.h"

#ifndef NN_MATRIX_IMPLEMENTATION
    #define NN_MATRIX_IMPLEMENTATION
    #include "nn_matrix.h"
#endif

extern void test_nn_matrix_alloc(void);
extern void test_nn_matrix_rand(void);
extern void test_nn_matrix_sum(void);
extern void test_nn_matrix_dot(void);
extern void test_nn_matrix_sigmoid(void);

#endif /* NN_MATRIX_TESTS_H_ */

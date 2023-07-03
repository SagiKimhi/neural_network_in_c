#ifndef NN_TESTS_H_
#define NN_TESTS_H_

#include "nn_common_includes.h"

#ifndef NN_IMPLEMENTATION
    #define NN_IMPLEMENTATION
    #include "nn.h"
#endif

extern void test_nn_print(void);
extern void test_nn_rand(void);
extern void test_nn_forward(void);
extern void test_nn_save_and_load(void);
extern void test_nn_with_xor_model(void);

#endif /* NN_TESTS_H_ */

#ifndef NN_TRAINING_SET_TESTS_C_
#define NN_TRAINING_SET_TESTS_C_

#include <nn_training_set_tests.h>

void test_nn_ts_alloc_and_macros(void)
{
    nn_training_set_t ts = nn_ts_alloc(10);
    
    nn_ts_print(ts, "ts_after_allocation");

    for (size_t i = 0; i < NN_TS_SIZE(ts); i++) {
        NN_TS_INPUT_AT(ts, i)       = i;
        NN_TS_EXP_OUTPUT_AT(ts, i)  = i * 2;
    }

    nn_ts_print(ts, "ts_after_setting_values");
    
    nn_ts_free(ts);
}


#endif /* NN_TRAINING_SET_TESTS_C_ */

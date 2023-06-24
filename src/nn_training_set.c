#ifndef NN_TRAINING_SET_C_
#define NN_TRAINING_SET_C_

#include <nn_training_set.h>


/* -------------------
 * Allocation Methods:
 * ------------------- */

nn_training_set_t nn_ts_alloc(size_t nof_training_datas)
{
    return (
        nn_matrix_alloc(nof_training_datas, NN_TD_ARR_SIZE)
    );
}

extern void nn_ts_free(nn_training_set_t s)
{
    nn_matrix_free(s);
}

/* ----------------
 * Utility Methods:
 * ---------------- */
void nn_ts_print(nn_training_set_t set, char *name)
{
    NN_ASSERT(set.data);
    NN_ASSERT(set.cols == NN_TD_ARR_SIZE);

    if (name)
        printf("%s = ", name);

    printf("{\n");

    for (size_t td = 0; td < NN_TS_SIZE(set); td++) {
        printf(
            "\t[ Input = %0f, Expected_output = %0f ]%s\n",
            NN_TS_INPUT_AT(set, td), 
            NN_TS_EXP_OUTPUT_AT(set, td),
            (td + 1 < NN_TS_SIZE(set) ? ",": "")
        );
    }

    printf("}\n");
}


#endif /* NN_TRAINING_SET_C_ */

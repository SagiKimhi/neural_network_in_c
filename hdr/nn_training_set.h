#ifndef NN_TRAINING_SET_H_
#define NN_TRAINING_SET_H_

/* ---------
 * Includes:
 * --------- */
#include <nn_common_includes.h>
#include <nn_matrix.h>


/* --------
 * Defines:
 * -------- */
#define NN_TD_ARR_SIZE              2
#define NN_TD_INPUT_IDX             0
#define NN_TD_EXPECTED_OUTPUT_IDX   1

#define NN_TS_INPUT_AT(set, i)\
        NN_MATRIX_AT((set), (i), NN_TD_INPUT_IDX)

#define NN_TS_EXP_OUTPUT_AT(set, i)\
    NN_MATRIX_AT((set), (i), NN_TD_EXPECTED_OUTPUT_IDX)

#define NN_TS_SIZE(ts) (ts).rows

#define NN_TS_PRINT(set) nn_training_set_print(set, #set)


/* ---------
 * Typedefs:
 * --------- */
typedef nn_matrix_t nn_training_set_t;


/* ----------------------
 * Function Declarations:
 * ---------------------- */  

/* Allocation Methods */
extern nn_training_set_t nn_ts_alloc(size_t nof_training_datas);
extern void nn_ts_free(nn_training_set_t s);

/* Utility Methods */
extern void nn_ts_print(nn_training_set_t set, char *name);


#endif /* NN_TRAINING_SET_H_ */

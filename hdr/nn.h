#ifndef NN_H_
#define NN_H_

/* ---------
 * Includes:
 * --------- */
#include <nn_common_includes.h>
#include <nn_matrix.h>


/* --------
 * Defines:
 * -------- */
#define NN_PRINT(nn)        nn_print(nn, #nn)
#define NN_INPUT(nn)        ( (nn).activations[0] )
#define NN_OUTPUT(nn)       ( (nn).activations[(nn).nof_layers] )
#define NN_SIZEOF_ARR(arr)  ( sizeof((arr)) / sizeof((arr)[0]) )

#ifndef NN_ACT_FUNC
    #define NN_ACT_FUNC ACT_SIGMOID
#endif


/* ---------
 * Typedefs:
 * --------- */
typedef struct {
    size_t      nof_layers;
    nn_matrix_t *weights;
    nn_matrix_t *biases;
    nn_matrix_t *activations;
} nn_t;

typedef struct {
    size_t arch_len;
    size_t *arch;
} nn_arch_t;


/* ----------------------
 * Function Declarations:
 * ---------------------- */  

/* Allocation Methods */
extern nn_t nn_alloc(size_t layers_architecture[], size_t architecture_arr_size);
extern void nn_free(nn_t nn);

/* Value Setters: */
extern void nn_rand(nn_t nn, float low, float high);
extern void nn_memset(nn_t nn, float val);

/* NN Methods: */
extern void nn_learn(nn_t nn, nn_t grad, float rate);
extern void nn_back_propagation(
    nn_t nn, nn_t grad, 
    nn_matrix_t ts_in, nn_matrix_t ts_out
);
extern void nn_finite_difference(
    nn_t nn, nn_t grad, float eps,
    nn_matrix_t ts_in, nn_matrix_t ts_out
);
extern float nn_cost(nn_t nn, nn_matrix_t ts_in, nn_matrix_t ts_out);
extern void nn_forward(nn_t nn);


/* Util Methods */
extern void nn_print(nn_t nn, const char *name);
extern void nn_fprint(FILE *stream, nn_t nn, const char *name);
extern void nn_save_model(FILE *fp, nn_t nn);
extern nn_t nn_load_model(FILE *fp);


#endif /* NN_H_ */

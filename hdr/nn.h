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
#define NN_SIZEOF_ARR(arr) ( sizeof((arr)) / sizeof((arr)[0]) )
#define NN_PRINT(nn) nn_print(nn, #nn)


/* ---------
 * Typedefs:
 * --------- */

typedef struct {
    size_t      nof_layers;
    nn_matrix_t *weights;
    nn_matrix_t *biases;
    nn_matrix_t *activations;
} nn_t;


/* ----------------------
 * Function Declarations:
 * ---------------------- */  

/* Allocation Methods */
extern nn_t nn_alloc(size_t layers_architecture[], size_t architecture_arr_size);
extern void nn_free(nn_t nn);


/* Util Methods */
extern void nn_print(nn_t nn, char *name);


#endif /* NN_H_ */

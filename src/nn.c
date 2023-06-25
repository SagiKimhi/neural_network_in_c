#ifndef NN_C_
#define NN_C_

#include <nn.h>


/* -------------------
 * Allocation Methods:
 * -------------------*/
nn_t nn_alloc(size_t layers_architecture[], size_t architecture_arr_size)
{
    NN_ASSERT(layers_architecture);
    NN_ASSERT(architecture_arr_size > 1);

    nn_t nn;

    nn.nof_layers   = architecture_arr_size - 1;
    nn.weights      = malloc(sizeof(*nn.weights) * nn.nof_layers);
    nn.biases       = malloc(sizeof(*nn.biases) * nn.nof_layers);
    nn.activations  = malloc(sizeof(*nn.activations) * architecture_arr_size);

    NN_ASSERT(nn.weights);
    NN_ASSERT(nn.biases);
    NN_ASSERT(nn.activations);

    nn.activations[0]   = nn_matrix_alloc(1, layers_architecture[0]);

    for (size_t i = 1; i < architecture_arr_size; i++) {
        nn.activations[i]   = nn_matrix_alloc(1, layers_architecture[i]);
        nn.biases[i-1]      = nn_matrix_alloc(1, layers_architecture[i]);
        nn.weights[i-1]     = nn_matrix_alloc(nn.activations[i].cols, layers_architecture[i]);
    }   

    return nn;
}

void nn_free(nn_t nn)
{
    NN_ASSERT(nn.nof_layers);
    NN_ASSERT(nn.weights);
    NN_ASSERT(nn.biases);
    NN_ASSERT(nn.activations);

    nn_matrix_free(nn.activations[0]);

    for (size_t i = 1; i < (nn.nof_layers + 1); i++) {
        nn_matrix_free(nn.weights[i-1]);
        nn_matrix_free(nn.biases[i-1]);
        nn_matrix_free(nn.activations[i]);
    }

    free(nn.weights);
    free(nn.biases);
    free(nn.activations);
}


/* -------------
 * Util Methods:
 * ------------- */
void nn_print(nn_t nn, char *name)
{
    if (name)
        printf("%s = ", name);

    printf("{\n");
    
    for (size_t i = 0; i < nn.nof_layers; i++) {
        NN_MATRIX_PRINT(nn.weights[i]);
        NN_MATRIX_PRINT(nn.biases[i]);
    }
}


#endif /* NN_C_ */

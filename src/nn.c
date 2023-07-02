#ifndef NN_C_
#define NN_C_

#include "nn.h"


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
        nn.weights[i-1]     = nn_matrix_alloc(nn.activations[i-1].cols, layers_architecture[i]);
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


/* --------------
 * Value Setters: 
 * -------------- */
void nn_rand(nn_t nn, float low, float high)
{
    NN_ASSERT(nn.nof_layers);
    NN_ASSERT(nn.weights);
    NN_ASSERT(nn.biases);

    for (size_t i = 0; i < nn.nof_layers; i++) {
        nn_matrix_rand(nn.weights[i], low, high);
        nn_matrix_rand(nn.biases[i], low, high);
    }
}

void nn_memset(nn_t nn, float val)
{
    for (size_t i = 0; i < nn.nof_layers; i++) {
        nn_matrix_memset(nn.weights[i], val);
        nn_matrix_memset(nn.biases[i], val);
        nn_matrix_memset(nn.activations[i], val);
    }

    nn_matrix_memset(NN_OUTPUT(nn), val);
}


/* -----------
 * NN Methods:
 * ----------- */
void nn_learn(nn_t nn, nn_t grad, float rate)
{
    for (size_t i = 0; i < nn.nof_layers; i++) {
        for (size_t w_row = 0; w_row < nn.weights[i].rows; w_row++) {
            for (size_t w_col = 0; w_col < nn.weights[i].cols; w_col++) {
                NN_MATRIX_AT(nn.weights[i], w_row, w_col) -= (
                    rate * NN_MATRIX_AT(grad.weights[i], w_row, w_col)
                );
            }
        }
    }

    for (size_t i = 0; i < nn.nof_layers; i++) {
        for (size_t b_row = 0; b_row < nn.biases[i].rows; b_row++) {
            for (size_t b_col = 0; b_col < nn.biases[i].cols; b_col++) {
                NN_MATRIX_AT(nn.biases[i], b_row, b_col) -= (
                    rate * NN_MATRIX_AT(grad.biases[i], b_row, b_col)
                );
            }
        }
    }
}

void nn_back_propagation(
    nn_t nn, nn_t grad, 
    nn_matrix_t ts_in, nn_matrix_t ts_out
)
{
    size_t n_samples;

    NN_ASSERT(ts_in.rows == ts_out.rows);
    NN_ASSERT(ts_out.cols == NN_OUTPUT(nn).cols);

    nn_memset(grad, 0);
    n_samples = ts_in.rows;

#ifdef NN_BACKPROP_TRADITIONAL
    float out_multiplier = 2;
    float pd_multiplier = 1;
#else
    float out_multiplier = 1;
    float pd_multiplier = 2;
#endif /* NN_BACKPROP_TRADITIONAL */

    for (size_t sample = 0; sample < n_samples; sample++) {
        nn_matrix_copy(NN_INPUT(nn), nn_matrix_row(ts_in, sample));
        nn_forward(nn);

        for (size_t l = 0; l <= nn.nof_layers; l++) {
            nn_matrix_memset(grad.activations[l], 0);
        }

        for (size_t out_idx = 0; out_idx < ts_out.cols; out_idx++) {
            NN_MATRIX_AT(NN_OUTPUT(grad), 0, out_idx) = (
                out_multiplier * (
                    NN_MATRIX_AT(NN_OUTPUT(nn), 0, out_idx) - 
                    NN_MATRIX_AT(ts_out, sample, out_idx)
                )
            );
        }

        for (size_t l = nn.nof_layers; l > 0; l--) {
            /* 
             * j = weight/bias matrix column
             * k = weight matrix row
             * */
            for (size_t j = 0; j < nn.activations[l].cols; j++) {
                float a = NN_MATRIX_AT(nn.activations[l], 0, j);
                float cost_a = NN_MATRIX_AT(grad.activations[l], 0, j);
                float deriv_a = dactf(a, NN_ACT_FUNC);

                NN_MATRIX_AT(grad.biases[l-1], 0, j) += (
                    pd_multiplier * cost_a * deriv_a
                );

                for (size_t k = 0; k < nn.activations[l-1].cols; k++) {
                    float prev_a = NN_MATRIX_AT(nn.activations[l-1], 0, k);
                    float aw = NN_MATRIX_AT(nn.weights[l-1], k, j);

                    NN_MATRIX_AT(grad.weights[l-1], k, j) += (
                        pd_multiplier * cost_a * deriv_a * prev_a
                    );
                    NN_MATRIX_AT(grad.activations[l-1], 0, k) += (
                        pd_multiplier * cost_a * deriv_a * aw
                    );
                }
            }
        }
    }

    for (size_t l = 0; l < grad.nof_layers; l++) {
        for (size_t col = 0; col < grad.weights[l].cols; col++)
            for (size_t row = 0; row < grad.weights[l].rows; row++)
                NN_MATRIX_AT(grad.weights[l], row, col) /= n_samples;

        for (size_t col = 0; col < grad.biases[l].cols; col++)
            for (size_t row = 0; row < grad.biases[l].rows; row++)
                NN_MATRIX_AT(grad.biases[l], row, col) /= n_samples;
    }
}

void nn_finite_difference(
    nn_t nn, nn_t grad, float eps,
    nn_matrix_t ts_in, nn_matrix_t ts_out
)
{
    float saved;
    float original_cost;

    original_cost = nn_cost(nn, ts_in, ts_out);

    for (size_t i = 0; i < nn.nof_layers; i++) {
        for (size_t w_row = 0; w_row < nn.weights[i].rows; w_row++) {
            for (size_t w_col = 0; w_col < nn.weights[i].cols; w_col++) {
                saved = NN_MATRIX_AT(nn.weights[i], w_row, w_col);
                
                NN_MATRIX_AT(nn.weights[i], w_row, w_col)  += eps;
                NN_MATRIX_AT(grad.weights[i], w_row, w_col) = (
                    ( nn_cost(nn, ts_in, ts_out) - original_cost ) / eps
                );
                NN_MATRIX_AT(nn.weights[i], w_row, w_col)   = saved;
            }
        }
    }

    for (size_t i = 0; i < nn.nof_layers; i++) {
        for (size_t b_row = 0; b_row < nn.biases[i].rows; b_row++) {
            for (size_t b_col = 0; b_col < nn.biases[i].cols; b_col++) {
                saved = NN_MATRIX_AT(nn.biases[i], b_row, b_col);
                
                NN_MATRIX_AT(nn.biases[i], b_row, b_col)  += eps;
                NN_MATRIX_AT(grad.biases[i], b_row, b_col) = (
                    ( nn_cost(nn, ts_in, ts_out) - original_cost ) / eps
                );
                NN_MATRIX_AT(nn.biases[i], b_row, b_col)   = saved;
            }
        }
    }
}

float nn_cost(nn_t nn, nn_matrix_t ts_in, nn_matrix_t ts_out)
{
    float   cost, distance;
    size_t  n_samples;

    NN_ASSERT(ts_in.rows == ts_out.rows);
    NN_ASSERT(ts_out.cols == NN_OUTPUT(nn).cols);

    cost        = 0;
    distance    = 0;
    n_samples   = ts_in.rows;

    for (size_t sample = 0; sample < n_samples; sample++) {
        nn_matrix_t x = nn_matrix_row(ts_in, sample);
        nn_matrix_t y = nn_matrix_row(ts_out, sample);

        nn_matrix_copy(NN_INPUT(nn), x);
        nn_forward(nn);

        for (size_t out_idx = 0; out_idx < ts_out.cols; out_idx++) {
            distance = (
                NN_MATRIX_AT(NN_OUTPUT(nn), 0, out_idx) - 
                NN_MATRIX_AT(y,             0, out_idx)
            );
            cost    += ( distance * distance );
        }
    }

    return ( cost / n_samples );
}

void nn_forward(nn_t nn)
{
    NN_ASSERT(nn.nof_layers);
    NN_ASSERT(nn.weights);
    NN_ASSERT(nn.biases);
    NN_ASSERT(nn.activations);

    for (size_t i = 0; i < nn.nof_layers; i++) {
        nn_matrix_dot(nn.activations[i+1], nn.activations[i], nn.weights[i]);
        nn_matrix_sum(nn.activations[i+1], nn.biases[i]);
        nn_matrix_actf(nn.activations[i+1], NN_ACT_FUNC);
    }
}


/* -------------
 * Util Methods:
 * ------------- */
void nn_print(nn_t nn, const char *name)
{
    nn_fprint(stdout, nn, name);
}

void nn_fprint(FILE *stream, nn_t nn, const char *name)
{
    char buf[256] = {0};

    if (name)
        fprintf(stream, "%s = ", name);

    fprintf(stream, "{\n");
    
    for (size_t i = 0; i < nn.nof_layers; i++) {
        NN_ASSERT(snprintf(buf, sizeof(buf), "%s%zu%s", "weight[", i, "]"));
        nn_matrix_fprint(stream, nn.weights[i], buf, 4);
        NN_ASSERT(snprintf(buf, sizeof(buf), "%s%zu%s", "bias[", i, "]"));
        nn_matrix_fprint(stream, nn.biases[i], buf, 4);
    }

    fprintf(stream, "}\n");
}

void nn_save_model(FILE *fp, nn_t nn)
{
    NN_ASSERT(fp);
    NN_ASSERT(!ferror(fp));
    NN_ASSERT(nn.nof_layers);
    NN_ASSERT(nn.activations && nn.weights && nn.biases);

    fwrite(&nn.nof_layers, sizeof(nn.nof_layers), 1, fp);
    
    nn_matrix_save(fp, NN_INPUT(nn));

    for (size_t layer = 1; layer < nn.nof_layers + 1; layer++) {
        nn_matrix_save(fp, nn.activations[layer]);
        nn_matrix_save(fp, nn.weights[layer - 1]);
        nn_matrix_save(fp, nn.biases[layer - 1]);
    }
}

nn_t nn_load_model(FILE *fp)
{
    nn_t nn;

    NN_ASSERT(fp);
    NN_ASSERT(!ferror(fp));

    fread(&nn.nof_layers, sizeof(nn.nof_layers), 1, fp);

    NN_ASSERT(!ferror(fp));

    nn.activations  = NN_MALLOC(sizeof(*nn.activations) * (nn.nof_layers + 1));
    nn.weights      = NN_MALLOC(sizeof(*nn.weights) * nn.nof_layers);
    nn.biases       = NN_MALLOC(sizeof(*nn.biases) * nn.nof_layers);

    NN_ASSERT(nn.activations && nn.weights && nn.biases);

    NN_INPUT(nn) = nn_matrix_load(fp);

    for (size_t layer = 1; layer < nn.nof_layers + 1; layer++) {
        nn.activations[layer]   = nn_matrix_load(fp);
        nn.weights[layer - 1]   = nn_matrix_load(fp);
        nn.biases[layer - 1]    = nn_matrix_load(fp);
    }

    return nn;
}


#endif /* NN_C_ */

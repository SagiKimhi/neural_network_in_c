
//#define NN_BACKPROP_TRADITIONAL
#define NN_ACT_FUNC ACT_SIGMOID
#define NN_ELU_PARAM 0.01f

#define NN_RENDER_IMPLEMENTATION
#include "nn_render.h"

#define BITS    (6)
#define MAX_N   (1 << BITS)

/* General Window Constants */

void init_ts_values(nn_matrix_t *ts_inout);
void print_model_results(nn_t nn, nn_matrix_t ts_in, nn_matrix_t ts_out);

int main(int argc, char **argv)
{
    srand(time(0));

    size_t      arch[]      = { 2 * BITS, 3 * BITS, 3 * BITS, 3 * BITS, 3 * BITS, BITS + 1};
    size_t      arch_len    = NN_SIZEOF_ARR(arch);
    nn_render_cfg_t render_cfg  = {
        .nn_arch                    = {.arch = arch, .arch_len = arch_len},
        .nn_rate                    = 1e-1,
        .rand_low                   = -1,
        .rand_high                  = 1,
        .batch_size                 = 125,
        .batches_per_frame          = 0x20,
        .shuffle_rows               = true,
        .load_data_fn               = &init_ts_values,
        .optional_render_results_fn = NULL,
        .optional_print_results_fn  = &print_model_results,
    };

    nn_render_gui(render_cfg);

    return 0;
}


void init_ts_values(nn_matrix_t *ts_inout)
{
    size_t n_samples = MAX_N * MAX_N;

    if (!ts_inout->data)
        *ts_inout = nn_matrix_alloc(n_samples, 2 * BITS + BITS + 1);

    for (size_t n = 0; n < n_samples; n++) {
        size_t x = n / MAX_N;
        size_t y = n % MAX_N;
        size_t z = x + y;

        for (size_t col = 0; col < BITS; col++) {
            NN_MATRIX_AT(*ts_inout, n, col)         = ( (x >> (BITS -1 - col) ) & 1);
            NN_MATRIX_AT(*ts_inout, n, BITS + col)  = ( (y >> (BITS -1 - col) ) & 1);
            NN_MATRIX_AT(*ts_inout, n, 2*BITS+col)        = ( 
                z < MAX_N ? ( (z >> (BITS - 1 - col) ) & 1 ): 0
            );
        }

        if (z >= MAX_N)
            NN_MATRIX_AT(*ts_inout, n, 2*BITS+BITS) = 1;
    }
}


void print_model_results(nn_t nn, nn_matrix_t ts_in, nn_matrix_t ts_out)
{
    const float pos_eps         = 85e-2;
    const float neg_eps         = 15e-2;
    size_t      n_samples   = MAX_N * MAX_N;
    size_t      err_cnt     = 0;

    printf("\n\n-------------------------------------------------------\n\n");

    for (size_t n = 0; n < n_samples; n++) {
        size_t  x                   = n / MAX_N;
        size_t  y                   = n % MAX_N;
        size_t  expected_result     = x + y;
        size_t  actual_result       = 0;
        size_t  actual_undetermined = 0;

        printf("%zu + %zu = ", x, y);

        for (size_t col = 0; col < BITS; col++) {
            NN_MATRIX_AT(NN_INPUT(nn), 0, col)          = ( 
                (x >> (BITS -1 - col) ) & 1 
            );
            NN_MATRIX_AT(NN_INPUT(nn), 0, BITS + col)   = ( 
                (y >> (BITS -1 - col) ) & 1
            );
        }

        nn_forward(nn);

        for (size_t col = 0; col < NN_OUTPUT(nn).cols-1; col++) {
            float   res = NN_MATRIX_AT(NN_OUTPUT(nn), 0, col);

            if (res >= pos_eps)
                actual_result |= ( 1 << (BITS - 1 - col) );
            else if (res <= neg_eps)
                continue;
            else
                actual_undetermined = 1;
        }

        actual_undetermined |= (
            NN_MATRIX_AT(NN_OUTPUT(nn), 0, BITS) < pos_eps &&
            NN_MATRIX_AT(NN_OUTPUT(nn), 0, BITS) > neg_eps
        );
                

        if (expected_result >= MAX_N) {
            if (actual_undetermined) {
                printf("<Expected: Overflow | Actual: X (Undetermined)>\n");
                err_cnt++;
            }
            else if (!actual_result && NN_MATRIX_AT(NN_OUTPUT(nn), 0, BITS) >= pos_eps) {
                printf("Overflow\n");
            }
            else {
                printf("<Expected: Overflow | Actual: %zu>\n", actual_result);
                err_cnt++;
            }
        
            continue;
        }

        if (actual_undetermined) {
            printf("<Expected: %zu | Actual: X (Undetermined)>\n", expected_result);
            err_cnt++;
            continue;
        }

        if (expected_result != actual_result) {
            printf("<Expected: %zu | Actual: %zu>\n", expected_result, actual_result);
            err_cnt++;
            continue;
        }

        printf("%zu\n", actual_result);
    }

    printf("\nAll inputs were tested.\n");

    if (!err_cnt)
        printf("Great Success!\n\n");
    else
        printf("BIG F! Shame!\n\n");

    printf("Total number of model errors caught was: %zu/%zu\n", err_cnt, n_samples);
    printf("\n\n-------------------------------------------------------\n\n");
}

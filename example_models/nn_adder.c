#include <nn.h>

#define BITS    (4)
#define MAX_N   (1 << BITS)

void init_ts_values(nn_matrix_t ts_in, nn_matrix_t ts_out);
void print_model_results(nn_t nn, nn_matrix_t ts_in, nn_matrix_t ts_out);

int main(int argc, char **argv)
{
    srand(time(0));

    char        buf[2];
    size_t      arch[]      = { 2 * BITS, 3 * BITS, 2 * BITS, BITS + 1};
    size_t      arch_len    = NN_SIZEOF_ARR(arch);
    float       rate        = 1;
    nn_t        nn          = nn_alloc(arch, arch_len);
    nn_t        gradient    = nn_alloc(arch, arch_len);
    nn_matrix_t ts          = nn_matrix_alloc(MAX_N * MAX_N, BITS * 3 + 1);
    nn_matrix_t ts_in       = {
        .cols   = 2 * BITS,
        .rows   = ts.rows,
        .stride = ts.stride,
        .data   = ts.data,
    };
    nn_matrix_t ts_out      = {
        .cols   = BITS + 1,
        .rows   = ts.rows,
        .stride = ts.stride,
        .data   = (ts.data + ts_in.cols),
    };

    nn_rand(nn, 0, 1);
    init_ts_values(ts_in, ts_out);

    printf("-------------------------------------------------------\n");
    NN_MATRIX_PRINT(ts_in);
    NN_MATRIX_PRINT(ts_out);
    NN_PRINT(nn);
    printf("\nPrints above are the initial training set and nn model values.\n");
    printf("-------------------------------------------------------\n\n");
    printf("Please press Enter/<CR> to approve or <Ctrl+c> to exit...\n");
    fgets(buf, 2, stdin);


    printf("\n\nTeaching My AI Model how to add bits together :O \n\n");
    printf("-------------------------------------------------------\n");
    printf("Cost function value before training: %f\n", nn_cost(nn, ts_in, ts_out));
    printf("Model results before training:\n");
    print_model_results(nn, ts_in, ts_out);
    printf("-------------------------------------------------------\n\n");

    printf("Press enter to start the training process...\n");
    fgets(buf, 2, stdin);

    for (size_t i = 0; i < 100*1000; i++) {
        nn_back_propagation(nn, gradient, ts_in, ts_out);
        nn_learn(nn, gradient, rate);
        
        if (!(i % 150))
            printf("%zu: cost = %f\n", i, nn_cost(nn, ts_in, ts_out));
    }

    printf("\n\n-------------------------------------------------------\n");
    printf("Cost function value after training: %f\n", nn_cost(nn, ts_in, ts_out));
    printf("Model results after training:\n");
    print_model_results(nn, ts_in, ts_out);
    printf("-------------------------------------------------------\n\n");

    nn_free(nn);
    nn_free(gradient);

    return 0;
}

void init_ts_values(nn_matrix_t ts_in, nn_matrix_t ts_out)
{
    size_t n_samples = MAX_N * MAX_N;

    NN_ASSERT(ts_in.rows == n_samples);
    NN_ASSERT(ts_out.rows == n_samples);

    for (size_t n = 0; n < n_samples; n++) {
        size_t x = n / MAX_N;
        size_t y = n % MAX_N;
        size_t z = x + y;

        for (size_t col = 0; col < BITS; col++) {
            NN_MATRIX_AT(ts_in, n, col)         = ( (x >> (BITS -1 - col) ) & 1);
            NN_MATRIX_AT(ts_in, n, BITS + col)  = ( (y >> (BITS -1 - col) ) & 1);
            NN_MATRIX_AT(ts_out, n, col)        = ( 
                z < MAX_N ? ( (z >> (BITS - 1 - col) ) & 1 ): 0
            );
        }

        if (z >= MAX_N)
            NN_MATRIX_AT(ts_out, n, ts_out.cols-1) = 1;
    }
}

void print_model_results(nn_t nn, nn_matrix_t ts_in, nn_matrix_t ts_out)
{
    const float pos_eps         = 85e-2;
    const float neg_eps         = 15e-2;
    size_t      n_samples   = MAX_N * MAX_N;
    size_t      err_cnt     = 0;

    NN_ASSERT(ts_in.rows == n_samples);
    NN_ASSERT(ts_out.rows == n_samples);

    for (size_t n = 0; n < n_samples; n++) {
        size_t  x                   = n / MAX_N;
        size_t  y                   = n % MAX_N;
        size_t  expected_result     = x + y;
        size_t  actual_result       = 0;
        size_t  actual_undetermined = 0;

        nn_matrix_copy(NN_INPUT(nn), nn_matrix_row(ts_in, n));
        nn_forward(nn);

        printf("%zu + %zu = ", x, y);

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

}

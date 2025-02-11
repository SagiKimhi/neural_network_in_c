#include <unistd.h>

/* -------------
 * nn.h Defines:
 * ------------- */
#define NN_BACKPROP_TRADITIONAL
#define NN_ACT_FUNC ACT_P_RELU
#define NN_ELU_PARAM 0.01f
#define NN_RELU_PARAM 0.003f
#define NN_IMPLEMENTATION
#include "nn.h"

/* --------------------
 * nn_render.h defines:
 * -------------------- */
#define NN_RENDER_IMPLEMENTATION
#include "nn_render.h"

/* --------
 * Defines:
 * -------- */
#define REPORT_CMDLINE_ARG_ERR(flag) report_cmdline_arg_err(flag, #flag)

/* ---------
 * Typedefs:
 * --------- */
typedef enum {
    NOT_ENOUGH_ARGS_F = (1 << 0),
    INVALID_PUBLIC_KEY_FILE_PATH_F = (1 << 1),
    INVALID_PRIVATE_KEY_FILE_PATH_F = (1 << 2),
    INVALID_OPTIONAL_ARG_SYNTAX_F = (1 << 3),
    INVALID_OPTIONAL_ARG_LENGTH_F = (1 << 4),
    INVALID_KEYPAIR_PATH_LEN_F = (1 << 5),
} cmdline_args_err_flags;

/* ----------
 * Constants:
 * ---------- */
const size_t nn_input_size = 96;
const size_t nn_output_size = 384;
const uint8_t args_are_valid = 0;
const size_t min_nof_cmdline_args = 2;
const size_t keypair_path_arg_idx = 1;
const size_t optional_range_arg_idx = 2;
const size_t public_key_size_bytes = 563;
const size_t private_key_size_bytes = 2520;
const size_t max_optional_arg_len = 3;
const char public_key_postfix[] = ".enc";
const size_t max_priv_key_path_len
    = (BUFSIZ - NN_SIZEOF_ARR(public_key_postfix) - max_optional_arg_len);

/* --------------------
 * Function Prototypes:
 * -------------------- */
uint8_t
validate_cmdline_args(int argc, const char **argv);
static void
report_cmdline_arg_err(cmdline_args_err_flags flag, char *flag_name);
void
load_training_data(nn_matrix_t *ts_inout);
void
print_results(nn_t nn, nn_matrix_t ts_in, nn_matrix_t ts_out);

int
main(int argc, char **argv)
{
    srand(time(0));

    size_t arch[] = {nn_input_size, 23, 23, 23, 23, 23, 23, nn_output_size};
    size_t arch_len = NN_SIZEOF_ARR(arch);
    nn_render_cfg_t render_cfg = {
        .nn_arch = {.arch = arch, .arch_len = arch_len},
        .nn_rate = 1e-1,
        .rand_low = -1.3,
        .rand_high = 0,
        .batch_size = 20,
        .batches_per_frame = 0x80,
        .load_data_fn = &load_training_data,
        .optional_render_results_fn = NULL,
        .optional_print_results_fn = &print_results,
    };

    nn_render_gui(render_cfg);

    return EXIT_SUCCESS;
}

void
print_results(nn_t nn, nn_matrix_t ts_in, nn_matrix_t ts_out)
{
    char expected_buf[2048] = {0};
    char actual_buf[2048] = {0};
    size_t nof_errs = 0;

    NN_ASSERT(ts_in.rows == ts_out.rows);

    for (size_t i = 0; i < ts_out.rows; i++) {
        nn_matrix_copy(NN_INPUT(nn), nn_matrix_row(ts_in, i));
        nn_forward(nn);

        size_t prev_nof_errs = nof_errs;

        for (size_t j = 0; j < ts_out.cols; j++) {
            expected_buf[j]
                = (char)(((float)NN_MATRIX_AT(ts_out, i, j)) * CHAR_MAX);
            actual_buf[j]
                = (char)(((float)NN_MATRIX_AT(NN_OUTPUT(nn), i, j)) * CHAR_MAX);

            if (expected_buf[j] != actual_buf[j])
                nof_errs++;
        }

        if (prev_nof_errs == nof_errs)
            printf("Matched Output Sample:\n%s", actual_buf);
        else
            printf(
                "!!!!!!!!!!!!!!!Mismatch!!!!!!!!!!!!\n"
                "Expected Output:\n%s\nActual Output:\n%s\n",
                expected_buf,
                actual_buf);
    }

    if (!nof_errs)
        printf("Success! Expected and Actual outputs matched perfectly!\n");
    else
        printf(
            "Failue.\nTotal number of errors detected: %zu/%zu\n",
            nof_errs,
            ts_out.cols);
}

void
load_training_data(nn_matrix_t *ts_inout)
{
    char filepath[FILENAME_MAX + 1] = {0};
    uint8_t err_flag = 0;
    size_t decrypted_text_offset = 0;
    size_t encrypted_text_len = 0;
    size_t decrypted_text_len = 0;
    uint8_t encrypted_text_buf[BUFSIZ + 1] = {0};
    char decrypted_text_buf[BUFSIZ + 1] = {0};
    FILE *input_data_file = NULL;
    FILE *output_data_file = NULL;

    fprintf(
        stdout,
        "Please provide a path to the encrypted/decrypted file "
        "without the file extension.\n"
        "It is assumed that the decrypted file name ends with a .txt extension\n"
        "and that the decrypted file name ends with a .enc extension.\n"
        "Therefore, the if the name of decrypted text file is \"text.txt\""
        " then the filepath you provide should look something like this:\n"
        "<path to file directory>/text\n"
        "Considering the previous example, please provide a filepath: ");
    nn_io_get_line(filepath, FILENAME_MAX, stdin);
    fprintf(stdout, "\n");

    if (!(input_data_file = nn_io_fopen(filepath, ".enc", "r"))) {
        err_flag = 1;
    }

    if (!(output_data_file = nn_io_fopen(filepath, ".txt", "r"))) {
        err_flag = 1;
    }

    if (!err_flag
        && !fgets((char *)encrypted_text_buf, BUFSIZ, input_data_file)) {
        fprintf(
            stderr,
            "Failed to read encrypted text from file due to newline in begining of file or an empty file\n");
        err_flag = 1;
    }

    while (!err_flag
           && fgets(
               &decrypted_text_buf[decrypted_text_offset],
               BUFSIZ + 1 - decrypted_text_offset,
               output_data_file)) {
        size_t prev_offset = 0;

        if (!decrypted_text_offset) {
            decrypted_text_offset = strlen(decrypted_text_buf);
            continue;
        }

        prev_offset = decrypted_text_offset;
        decrypted_text_offset += strlen(&decrypted_text_buf[prev_offset]);
    }

    if (!err_flag && !encrypted_text_len) {
        encrypted_text_len = strlen((char *)encrypted_text_buf);
    }

    if (!err_flag && !decrypted_text_len) {
        decrypted_text_len = strlen(decrypted_text_buf);

        if (decrypted_text_len != decrypted_text_offset) {
            err_flag = 1;
            fprintf(
                stderr,
                "private_key_len: %zu, private_key_offset: %zu\n"
                "private_key: %s\n",
                decrypted_text_len,
                decrypted_text_offset,
                decrypted_text_buf);
        }
    }

    if (!err_flag && ts_inout->data == NULL) {
        *ts_inout = nn_matrix_alloc(1, nn_input_size + nn_output_size);
    }

    for (size_t j = 0; !err_flag && j < nn_input_size; j++)
        NN_MATRIX_AT(*ts_inout, 0, j)
            = (j < encrypted_text_len
                   ? ((encrypted_text_buf[j]) / (float)(unsigned)UINT8_MAX)
                   : 0);

    for (size_t j = 0; !err_flag && j < nn_output_size; j++)
        NN_MATRIX_AT(*ts_inout, 0, nn_input_size + j)
            = (j < decrypted_text_len
                   ? ((decrypted_text_buf[j]) / (float)CHAR_MAX)
                   : 0);

    fclose(input_data_file);
    fclose(output_data_file);
    input_data_file = NULL;
    output_data_file = NULL;

    if (err_flag) {
        fprintf(stderr, "err_flag was raised!\n");
        if (ts_inout->data) {
            nn_matrix_free(*ts_inout);
            ts_inout->data = NULL;
        }
        if (input_data_file)
            fclose(input_data_file);
        if (output_data_file)
            fclose(output_data_file);
    }
}

uint8_t
validate_cmdline_args(int argc, const char **argv)
{
    if (argc < min_nof_cmdline_args) {
        REPORT_CMDLINE_ARG_ERR(NOT_ENOUGH_ARGS_F);
        fprintf(
            stderr,
            "Usage: %s [path to file (without the file extension)]\n",
            argv[0]);
        return (!args_are_valid);
    }

    return args_are_valid;
}

static void
report_cmdline_arg_err(cmdline_args_err_flags flag, char *flag_name)
{
    fprintf(stderr, "Error: %s\n", flag_name);
    return;
}

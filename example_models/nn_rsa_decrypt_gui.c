#include <unistd.h>


/* -------------
 * nn.h Defines:
 * ------------- */
//#define NN_BACKPROP_TRADITIONAL
//#define NN_ACT_FUNC ACT_SIGMOID
#define NN_IMPLEMENTATION
#include "nn.h"

/* --------------------
 * nn_render.h defines:
 * -------------------- */
typedef void nn_load_data_func_t(char *data_filepath, nn_matrix_t *ts_in, nn_matrix_t *ts_out);
#define NN_LOAD_DATA_FUNC_T nn_load_data_func_t
#define NN_LOAD_DATA_FUNC_ARGS "", &ts_in, &ts_out
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
    NOT_ENOUGH_ARGS_F               = (1 << 0),
    INVALID_PUBLIC_KEY_FILE_PATH_F  = (1 << 1),
    INVALID_PRIVATE_KEY_FILE_PATH_F = (1 << 2),
    INVALID_OPTIONAL_ARG_SYNTAX_F   = (1 << 3),
    INVALID_OPTIONAL_ARG_LENGTH_F   = (1 << 4),
    INVALID_KEYPAIR_PATH_LEN_F      = (1 << 5),
} cmdline_args_err_flags;


/* ----------
 * Constants:
 * ---------- */
const size_t    nn_input_size           = 96;
const size_t    nn_output_size          = 384;
const uint8_t   args_are_valid          = 0;
const size_t    min_nof_cmdline_args    = 2;
const size_t    keypair_path_arg_idx    = 1;
const size_t    optional_range_arg_idx  = 2;
const size_t    public_key_size_bytes   = 563;
const size_t    private_key_size_bytes  = 2520;
const size_t    max_optional_arg_len    = 3;
const char      public_key_postfix[]    = ".enc";
const size_t    max_priv_key_path_len   = (
    BUFSIZ - NN_SIZEOF_ARR(public_key_postfix) - max_optional_arg_len
);

/* --------------------
 * Function Prototypes:
 * -------------------- */
uint8_t validate_cmdline_args(int argc, const char **argv);
static void report_cmdline_arg_err(cmdline_args_err_flags flag, char *flag_name);
void load_training_data(char *key_filepath, nn_matrix_t *ts_in, nn_matrix_t *ts_out);
void print_results(nn_t nn, nn_matrix_t ts_in, nn_matrix_t ts_out);


int main(int argc, char **argv)
{
    srand(time(0));
    nn_matrix_t ts_in, ts_out;
    
    load_training_data("encrypted_text.enc", &ts_in, &ts_out);

    if (ts_in.data == NULL || ts_out.data == NULL)
        return EXIT_FAILURE;

    size_t  arch[]              = {ts_in.cols, 23, 23, 23, 23, 23, 23, ts_out.cols};
    size_t  arch_len            = NN_SIZEOF_ARR(arch);

    nn_render_with_default_frames((nn_arch_t){arch_len, arch}, -1.2f, 1.2f, &load_training_data, &print_results);
    
    return EXIT_SUCCESS;
}



void print_results(nn_t nn, nn_matrix_t ts_in, nn_matrix_t ts_out)
{
    char expected_buf[2048] = {0};
    char actual_buf[2048]   = {0};
    size_t nof_errs         = 0;

    NN_ASSERT(ts_in.rows == ts_out.rows);

    for (size_t i = 0; i < ts_out.rows; i++) {
        nn_matrix_copy(NN_INPUT(nn), nn_matrix_row(ts_in, i));
        nn_forward(nn);

        size_t prev_nof_errs = nof_errs;

        for (size_t j = 0; j < ts_out.cols; j++) {
            expected_buf[j] = (char)( ((float)NN_MATRIX_AT(ts_out, i, j)) * CHAR_MAX );
            actual_buf[j] = (char)( ((float)NN_MATRIX_AT(NN_OUTPUT(nn), i, j)) * CHAR_MAX );
            
            if (expected_buf[j] != actual_buf[j])
                nof_errs++;
        }

        if (prev_nof_errs == nof_errs)
            printf("Matched Output Sample:\n%s", actual_buf);
        else
            printf("!!!!!!!!!!!!!!!Mismatch!!!!!!!!!!!!\n"\
                    "Expected Output:\n%s\nActual Output:\n%s\n",
                    expected_buf, actual_buf
            );

    }

    if (!nof_errs)
        printf("Success! Expected and Actual outputs matched perfectly!\n");
    else
        printf("Failue.\nTotal number of errors detected: %zu/%zu\n",
                nof_errs, ts_out.cols );
}


void load_training_data(char *key_filepath, nn_matrix_t *ts_in, nn_matrix_t *ts_out)
{

    uint8_t err_flag                        = 0;
    size_t  decrypted_text_offset           = 0;
    size_t  encrypted_text_len              = 0;
    size_t  decrypted_text_len              = 0;
    uint8_t encrypted_text_buf[BUFSIZ + 1]  = {0};
    char    decrypted_text_buf[BUFSIZ + 1]  = {0};
    FILE    *input_data_file                = NULL;
    FILE    *output_data_file               = NULL;

    if (!(input_data_file = nn_io_fopen(key_filepath, ".enc", "r"))) {
        err_flag = 1;
    }

    if (!(output_data_file = nn_io_fopen(key_filepath, ".txt", "r"))) {
        err_flag = 1;
    }

    if (!err_flag && !fgets((char*)encrypted_text_buf, BUFSIZ, input_data_file)) {
        fprintf(stderr, "Failed to read encrypted text from file due to newline in begining of file or an empty file\n");
        err_flag = 1;
    }

    while (!err_flag && fgets(&decrypted_text_buf[decrypted_text_offset], BUFSIZ + 1 - decrypted_text_offset, output_data_file)) {
        size_t prev_offset = 0;

        if (!decrypted_text_offset) {
            decrypted_text_offset = strlen(decrypted_text_buf);
            continue;
        }

        prev_offset = decrypted_text_offset;
        decrypted_text_offset += strlen(&decrypted_text_buf[prev_offset]);
    }

    if (!err_flag && !encrypted_text_len) {
        encrypted_text_len = strlen((char*)encrypted_text_buf);
    }
    
    if (!err_flag && !decrypted_text_len) {
        decrypted_text_len = strlen(decrypted_text_buf);

        if (decrypted_text_len != decrypted_text_offset) {
            err_flag = 1;
            fprintf(stderr, "private_key_len: %zu, private_key_offset: %zu\n"\
                    "private_key: %s\n",
                    decrypted_text_len, decrypted_text_offset, decrypted_text_buf);
        }
    }

    if (ts_in->data == NULL) {
        *ts_in = nn_matrix_alloc(1, nn_input_size);
    }

    if (ts_out->data == NULL) {
        *ts_out = nn_matrix_alloc(1, nn_output_size);
    }

    for (size_t j = 0; j < ts_in->cols; j++)
        NN_MATRIX_AT(*ts_in, 0, j) = (j < encrypted_text_len ? ( (encrypted_text_buf[j]) / (float)(unsigned)UINT8_MAX): 0);

    for (size_t j = 0; j < ts_out->cols; j++)
        NN_MATRIX_AT(*ts_out, 0, j) = ( j < decrypted_text_len ? ((decrypted_text_buf[j]) / (float)CHAR_MAX): 0);

    fclose(input_data_file);
    fclose(output_data_file);
    input_data_file = NULL;
    output_data_file = NULL;

    if (err_flag) {
        if (ts_in->data){
            nn_matrix_free(*ts_in);
            ts_out->data = NULL;
        }
        if (ts_out->data) {
            nn_matrix_free(*ts_out);
            ts_out->data = NULL;
        }
        if (input_data_file)
            fclose(input_data_file);
        if (output_data_file)
            fclose(output_data_file);
    }
}

uint8_t validate_cmdline_args(int argc, const char **argv)
{
    if (argc < min_nof_cmdline_args) {
        REPORT_CMDLINE_ARG_ERR(NOT_ENOUGH_ARGS_F);
        fprintf( stderr, 
            "Usage: %s [path to file (without the file extension)]\n",
            argv[0] 
        );
        return (!args_are_valid);
    }
    
    return args_are_valid;
}

static void report_cmdline_arg_err(cmdline_args_err_flags flag, char *flag_name)
{
    fprintf(stderr, "Error: %s\n", flag_name);
    return;
}

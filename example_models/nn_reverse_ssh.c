//#define NN_BACKPROP_TRADITIONAL
#include <nn.h>
typedef void nn_load_data_func_t(char *data_filepath, nn_matrix_t *ts_in, nn_matrix_t *ts_out, size_t range);
#define NN_LOAD_DATA_FUNC_T nn_load_data_func_t
#define NN_LOAD_DATA_FUNC_ARGS "", &ts_in, &ts_out, 1


/* ---------
 * Includes:
 * --------- */
#include <nn_render.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


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
const size_t    default_optional_range  = 1;
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
uint8_t validate_cmdline_args(int argc, const char **argv, size_t *optional_range);
uint8_t validate_filepath_arg(const char *keypair_path_arg, size_t optional_range);
uint8_t validate_optional_cmdline_arg(const char *keypair_path_arg, const char *optional_arg, size_t *range);
static void report_cmdline_arg_err(cmdline_args_err_flags flag, char *flag_name);
void load_training_data(char *key_filepath, nn_matrix_t *ts_in, nn_matrix_t *ts_out, size_t optional_range);
void print_results(nn_t nn, nn_matrix_t ts_in, nn_matrix_t ts_out);


int main(int argc, char **argv)
{
    srand(time(0));
    size_t      optional_arg_range  = default_optional_range;
    nn_matrix_t ts_in, ts_out;
    
    load_training_data("encrypted_text.enc", &ts_in, &ts_out, optional_arg_range);

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


void load_training_data(char *key_filepath, nn_matrix_t *ts_in, nn_matrix_t *ts_out, size_t optional_range)
{

    uint8_t err_flag                    = 0;
    size_t  private_key_offset          = 0;
    size_t  keypair_path_len            = 0;
    size_t  public_key_len              = 0;
    size_t  private_key_len             = 0;
    char    public_key_path[BUFSIZ+1]   = {0};
    char    private_key_path[BUFSIZ+1]  = {0};
    uint8_t public_key_buf[BUFSIZ + 1]  = {0};
    char    private_key_buf[BUFSIZ + 1] = {0};
    FILE    *input_data_file            = NULL;
    FILE    *output_data_file           = NULL;

    keypair_path_len = strnlen(key_filepath, max_priv_key_path_len + 1);

    snprintf(public_key_path, max_priv_key_path_len, "/home/kali/project/nn_training_data/ssh_data/data/text.enc");
    snprintf(private_key_path, max_priv_key_path_len, "/home/kali/project/nn_training_data/ssh_data/data/text.txt");

    for (size_t i = 1; i <= optional_range; i++) {
        if (optional_range != default_optional_range) {
            sprintf(&private_key_path[keypair_path_len-1], "%zu", i);
            sprintf(&public_key_path[keypair_path_len-1], "%zu%s", i, public_key_postfix);
        }

        if (!(input_data_file = fopen(public_key_path, "r"))) {
            fprintf(
                stderr, 
                "Failed to open public key file.\nPath was: %s\n",
                public_key_path
            );
            err_flag = 1;
            break;
        }

        if (!(output_data_file = fopen(private_key_path, "r"))) {
            fprintf(
                stderr, 
                "Failed to open private key file.\nPath was: %s\n",
                private_key_path
            );
            err_flag = 1;
            break;
        }

        if (!fgets(public_key_buf, BUFSIZ, input_data_file)) {
            fprintf(stderr, "Failed to read public key from file due to newline in begining of file or an empty file\n");
            err_flag = 1;
            break;
        }

        for (size_t private_key_iter = 0; fgets(&private_key_buf[private_key_offset], BUFSIZ + 1 - private_key_offset, output_data_file); private_key_iter++) {
            size_t prev_offset = 0;

            if (!private_key_offset) {
                private_key_offset = strlen(private_key_buf);
                continue;
            }

            prev_offset = private_key_offset;
            private_key_offset += strlen(&private_key_buf[prev_offset]);
        }

        if (!public_key_len) {
            public_key_len = strlen(public_key_buf);
        }
        else if (public_key_len != strlen(public_key_buf)) {
            fprintf(stderr, "Mismatch between public key sizes\nprevious length was: %zu, "\
                    "but length of key from file %s is %lu.\nPlease validate your keys and try again.\n",
                    public_key_len, public_key_path, strlen(public_key_buf));
            err_flag = 1;
            break;
        }
        
        if (!private_key_len) {
            private_key_len = strlen(private_key_buf);

            if (private_key_len != private_key_offset) {
                err_flag = 1;
                fprintf(stderr, "private_key_len: %zu, private_key_offset: %zu\n"\
                        "private_key: %s\n",
                        private_key_len, private_key_offset, private_key_buf);
                break;
            }
        }
        else if (private_key_len != private_key_offset) {
            fprintf(stderr, "Mismatch between private key sizes\nprevious length was: %zu, "\
                    "but length of key from file %s is %lu.\nPlease validate your keys and try again.\n",
                    private_key_len, private_key_path, strlen(private_key_buf));
            err_flag = 1;
            break;
        }

        private_key_offset = 0;

        if (ts_in->data == NULL) {
            *ts_in = nn_matrix_alloc(optional_range, nn_input_size);
        }

        if (ts_out->data == NULL) {
            *ts_out = nn_matrix_alloc(optional_range, nn_output_size);
        }

        for (size_t j = 0; j < ts_in->cols; j++)
            NN_MATRIX_AT(*ts_in, i-1, j) = (j < public_key_len ? ( (public_key_buf[j]) / (float)(unsigned)UINT8_MAX): 0);

        for (size_t j = 0; j < ts_out->cols; j++)
            NN_MATRIX_AT(*ts_out, i-1, j) = ( j < private_key_len ? ((private_key_buf[j]) / (float)CHAR_MAX): 0);

        fclose(input_data_file);
        fclose(output_data_file);
        input_data_file = NULL;
        output_data_file = NULL;
    }

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
        
        return;
    }
}

uint8_t validate_cmdline_args(int argc, const char **argv, size_t *optional_range)
{
    if (argc < min_nof_cmdline_args) {
        REPORT_CMDLINE_ARG_ERR(NOT_ENOUGH_ARGS_F);
        fprintf( stderr, 
            "Usage: %s [path to private key] {Optional range: 1,10}\n",
            argv[0] 
        );
        return (!args_are_valid);
    }

    if (argc > min_nof_cmdline_args) {
        return (
            validate_optional_cmdline_arg( argv[keypair_path_arg_idx], 
                argv[optional_range_arg_idx], optional_range )
        );
    }

    return validate_filepath_arg(argv[keypair_path_arg_idx], *optional_range);
}

uint8_t validate_optional_cmdline_arg(const char *keypair_path_arg, const char *optional_arg, size_t *range)
{
    NN_ASSERT(keypair_path_arg && optional_arg);

    size_t arg_len = strnlen(optional_arg, max_optional_arg_len + 1);

    if (arg_len > max_optional_arg_len) {
        REPORT_CMDLINE_ARG_ERR(INVALID_OPTIONAL_ARG_LENGTH_F);
        return (!args_are_valid);
    }
    
    *range = atoi(optional_arg);

    if (!(*range)) {
        *range = default_optional_range;
        REPORT_CMDLINE_ARG_ERR(INVALID_OPTIONAL_ARG_SYNTAX_F);
        return (!args_are_valid);
    }

    return validate_filepath_arg(keypair_path_arg, *range);
}

uint8_t validate_filepath_arg(const char *keypair_path_arg, size_t optional_range)
{
    size_t  keypair_path_len;
    char    public_key_buf[BUFSIZ+1]    = {0};
    char    private_key_buf[BUFSIZ+1]   = {0};

    keypair_path_len = strnlen(keypair_path_arg, max_priv_key_path_len + 1);

    if (!keypair_path_len || keypair_path_len > max_priv_key_path_len) {
        REPORT_CMDLINE_ARG_ERR(INVALID_KEYPAIR_PATH_LEN_F);
        return (!args_are_valid);
    }

    snprintf(private_key_buf, keypair_path_len + 1, "%s", keypair_path_arg);
    snprintf(public_key_buf, keypair_path_len + 1, "%s%s", keypair_path_arg, public_key_postfix);

    for (size_t i = 1; i <= optional_range; i++) {
        if (optional_range != default_optional_range) {
            sprintf(&private_key_buf[keypair_path_len-1], "%zu", i);
            sprintf(&public_key_buf[keypair_path_len-1], "%zu%s", i, public_key_postfix);
        }

        if (access(public_key_buf, R_OK)) {
            REPORT_CMDLINE_ARG_ERR(INVALID_PUBLIC_KEY_FILE_PATH_F);
            fprintf(stderr, "path was: %s\n", public_key_buf);
            return (!args_are_valid);
        }

        if (access(private_key_buf, R_OK)) {
            REPORT_CMDLINE_ARG_ERR(INVALID_PRIVATE_KEY_FILE_PATH_F);
            fprintf(stderr, "path was: %s\n", private_key_buf);
            return (!args_are_valid);
        }
    }

    return args_are_valid;
}

static void report_cmdline_arg_err(cmdline_args_err_flags flag, char *flag_name)
{
    fprintf(stderr, "Error: %s\n", flag_name);
    return;
}

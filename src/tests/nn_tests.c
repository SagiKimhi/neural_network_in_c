#include <nn_tests.h>

#define STRINGIFY(s) #s

static size_t arch[] = {2, 2, 1};
static size_t arch_len = NN_SIZEOF_ARR(arch);


void test_nn_print(void)
{
    for (size_t i = 0; i < arch_len; i++)
        printf("%s: %zu\n", STRINGIFY(arch[i]), arch[i]);

    printf("%s: %zu\n", STRINGIFY(arch_len), arch_len);

    nn_t nn = nn_alloc(arch, arch_len);


    NN_PRINT(nn);

    nn_free(nn);
}

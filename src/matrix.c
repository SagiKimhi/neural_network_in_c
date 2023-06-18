#define MATRIX_IMPLEMENTATION_
#include <matrix.h>

int main(int arg, char **argv)
{
    matrix_t m;
    
    m = matrix_alloc(10, 10);
    matrix_rand(m, 0, 10);
    matrix_print(m);
    matrix_free(m);

    return 0;
}

#include <nn_rand.h>

float rand_float(void)
{
    return (
        ((float) rand()) / ((float)RAND_MAX)
    );
}

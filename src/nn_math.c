#ifndef NN_MATH_C_
#define NN_MATH_C_

#include <nn_math.h>

float sigmoidf(float x)
{
    return ( 1.f / (1.f + expf(-x)) );
}

#endif /* NN_MATH_C_ */

#ifndef NN_MATH_C_
#define NN_MATH_C_

#include <nn_math.h>

float actf(float x, nn_act_func_enum actf)
{
    switch (actf) {
        case ACT_SIGMOID: return sigmoidf(x);
    }

    NN_ASSERT(0 && "Unreachable");
    return 0.0f;
}

float dactf(float y, nn_act_func_enum actf)
{
    switch (actf) {
        case ACT_SIGMOID: return dsigmoidf(y);
    }

    NN_ASSERT(0 && "Unreachable");
    return 0.0f;
}

float sigmoidf(float x)
{
    return ( 1.f / (1.f + expf(-x)) );
}

float dsigmoidf(float y)
{
    return ( y * (1 - y) );
}


#endif /* NN_MATH_C_ */

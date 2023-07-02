#ifndef NN_DEFINES_H_
#define NN_DEFINES_H_

#if !defined NN_MALLOC
    #include <stdlib.h>
    #define NN_MALLOC malloc
#endif /*NN_MALLOC*/

#if !defined NN_FREE
    #include <stdlib.h>
    #define NN_FREE free
#endif /* NN_FREE */

#if !defined NN_ASSERT
    #include <assert.h>
    #define NN_ASSERT assert
#endif /*NN_ASSERT*/

#if !defined NN_ACT_FUNC
    #define NN_ACT_FUNC ACT_ELU
#endif

#if !defined NN_RELU_PARAM
    #define NN_RELU_PARAM 0.01f
#endif /* NN_RELU_PARAM */

#if !defined NN_ELU_PARAM
    #define NN_ELU_PARAM 1.f
#endif

/* ---------
 * Typedefs:
 * --------- */
typedef enum {
    ACT_SIGMOID = 1,
    ACT_P_RELU = 2,
    ACT_TANH = 3,
    ACT_ELU = 4,
} nn_act_func_enum;



#endif /* NN_DEFINES_H_ */

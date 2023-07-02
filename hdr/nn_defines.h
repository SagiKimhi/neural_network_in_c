#ifndef NN_DEFINES_H_
#define NN_DEFINES_H_


#ifndef NN_MALLOC
    #include <stdlib.h>
    #define NN_MALLOC malloc
#endif /*NN_MALLOC*/

#ifndef NN_FREE
    #include <stdlib.h>
    #define NN_FREE free
#endif /* NN_FREE */

#ifndef NN_ASSERT
    #include <assert.h>
    #define NN_ASSERT assert
#endif /*NN_ASSERT*/


/* ---------
 * Typedefs:
 * --------- */
typedef enum {
    ACT_SIGMOID,
    ACT_P_RELU,
    ACT_TANH,
    ACT_ELU,
} nn_act_func_enum;



#endif /* NN_DEFINES_H_ */

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


#endif /* NN_DEFINES_H_ */

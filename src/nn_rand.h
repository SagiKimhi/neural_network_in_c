#ifndef NN_RAND_H_
#define NN_RAND_H_

/* ---------
 * Includes:
 * --------- */
#include "nn_common_includes.h"

/* ----------------------
 * Function Declarations:
 * ---------------------- */
extern float
rand_float(void); /* returns a random float */

#endif /* NN_RAND_H_ */

#ifdef NN_RAND_IMPLEMENTATION

float
rand_float(void)
{
    return (((float)rand()) / ((float)RAND_MAX));
}

#endif /* NN_RAND_IMPLEMENTATION */

#ifndef NN_MATH_H_
#define NN_MATH_H_

/* ---------
 * Includes:
 * --------- */
#include <nn_common_includes.h>
#include <math.h>


/* ----------------------
 * Function Declarations:
 * ---------------------- */  

extern float sigmoidf(float x);
extern float dsigmoidf(float y);
extern float actf(float x, nn_act_func_enum actf);
extern float dactf(float y, nn_act_func_enum actf);


#endif /* NN_MATH_H_ */

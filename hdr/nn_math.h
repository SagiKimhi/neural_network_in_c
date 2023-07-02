#ifndef NN_MATH_H_
#define NN_MATH_H_

/* ---------
 * Includes:
 * --------- */
#include "nn_common_includes.h"
#include <math.h>


/* ----------------------
 * Function Declarations:
 * ---------------------- */  
extern float actf(float x, nn_act_func_enum actf);
extern float dactf(float y, nn_act_func_enum actf);
extern float sigmoidf(float x);
extern float dsigmoidf(float y);
extern float p_reluf(float x);
extern float dp_reluf(float y);
extern float tanhf(float x);
extern float dtanhf(float y);
extern float eluf(float x);
extern float deluf(float y);
extern float sigluf(float x);
extern float dsigluf(float x);
extern float swishf(float x);
extern float dswishf(float x);


#endif /* NN_MATH_H_ */

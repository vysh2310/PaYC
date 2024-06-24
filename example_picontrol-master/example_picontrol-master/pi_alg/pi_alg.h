#ifndef PI_ALG_HEADER
#define PI_ALG_HEADER

#include "pi_global.h"
#include "pthread.h"

extern float32 duty_cycle;
extern float32 direction;

extern int8 duty_7bit;

/* Function declarations */
 void initialize_io(void);
 void process_inputs(const uint8* method_vec);
 float32 pi_alg(float32 Kp_gain, float32 Ki_gain, float32 dbg_val);

#endif

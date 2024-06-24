#ifndef PI_UTILS_HEADER
#define PI_UTILS_HEADER

#include "pi_global.h"

/* Function declarations */
float32 look_up_even(float32 u, map_data map, float32 *pYData, uint8 method);
void saturate(float32 *value, float32 lower, float32 upper);
void get_control_gains(float32* c_prop, float32* c_int);
uint8 write_log_buffer(float32 log_error, float32 state, float32 output, uint8 pos);
uint16 read_input_reg(void);
void set_output_reg(int8 duty,int8 direction);

#endif

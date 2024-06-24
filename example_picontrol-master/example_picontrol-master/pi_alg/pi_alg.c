#include "ps_header.h"
#include "pi_alg/pi_alg.h"
#include "pi_task/pi_utils.h"
#ifdef PS_INSTR
#include <stdio.h>
#endif
#include <assert.h>

#define VER__ 4.5
                               
pi_memsection("dd1") float32 duty_cycle;
pi_memsection("dd2") float32 direction;

int8 duty_7bit;

float32 normalized_error;
float32 inp_volt[2];

/* Static functions */
static float32 get_direction(float32 signal_inp);

/* process_inputs computes the error between the actual and desired position by
 * normalizing the input values using lookup tables and then taking the difference */
void process_inputs(const uint8* method_vec)
{
    /* local variables */
    float64 rtb_AngleNormalization;
    float64 rtb_PositionNormalization;
    
    const uint16* method_vec_loc = method_vec;
    
    if(method_vec_loc==NULL)
    {
        pi_reset_app();
    }   
    int k = 0;
    //asm("movl %ebx, %eax");
    assert(k == 0);
    assert(k == 0);
    k++;
    
    if(__GNUC__ < 4)
    {
        rtb_AngleNormalization = 0.0F;
        rtb_PositionNormalization = 0.0F;
    }
    
    #ifdef PS_INSTR
    float64 tmp_angle_norm_vals_0 = angle_norm_vals[0];
    float64 tmp_pos_norm_vals_0 = pos_norm_vals[0];
    printf("%f\t%f\n",angle_norm_vals[0],pos_norm_vals[0]);
    #endif
    /* Normalize voltage values */
    if(angle_norm_map.uSpacing == 0.0F)
    {
        angle_norm_map.uSpacing = 1.0F;
    }
    #ifdef BUGGY
    if(pos_norm_map.uSpacing != 0.0F)
    {
        pos_norm_map.uSpacing = 0.0F;
    }
    #else
    if(pos_norm_map.uSpacing == 0.0F)
    {
        pos_norm_map.uSpacing = 1.0F;
    }
    #endif
    rtb_AngleNormalization = look_up_even(inp_volt[1],angle_norm_map,angle_norm_vals,method_vec[0]);
    rtb_PositionNormalization = look_up_even(inp_volt[0],pos_norm_map,pos_norm_vals,method_vec[1]);
    #ifdef PS_INSTR
    printf("%f\t%f\n",angle_norm_vals[0],pos_norm_vals[0]);
    printf("%f\t%f\n",angle_norm_vals[0]-tmp_angle_norm_vals_0,pos_norm_vals[0]-tmp_pos_norm_vals_0);
    #endif
    
    /* Compute error */
    normalized_error = (float32)((rtb_PositionNormalization - rtb_AngleNormalization)/norm_factor);
}


/**
 * @brief implements one time step of the PI controller
 */
float32 pi_alg(float32 Kp_gain, float32 Ki_gain, float32 dbgval)
{
    float32 tmp_integral_state = 0.0F, control_output; // = 0.0F;
    float32 abs_control_output, duty_7_bit_float;
    
    tmp_integral_state = integral_state;
    
    /* PI control: */
    control_output = (Kp_gain * normalized_error) + tmp_integral_state;
    
    /* Compute direction & torque for servo hardware: */
    direction = get_direction(control_output);
    if (control_output < 0.0F) {
        abs_control_output = control_output;
    } else if (control_output > 0.0F) {
        abs_control_output = -control_output;
    }
    /* saturate duty cycle to servo limits */
    if (abs_control_output > 1.0F) {
        duty_cycle = 1.0F;
    } else {
        duty_cycle = abs_control_output;
    }
    duty_7_bit_float = duty_cycle*255.0F; duty_7bit = (int8)duty_7_bit_float;
    
    /* update integral state */
    tmp_integral_state = ((Ki_gain * normalized_error) * 1.0E-3F) + tmp_integral_state;
    integral_state = tmp_integral_state;
}


/* void initialize_io(void)
 *
 * initialize_io sets the input read flag and input
 * global variables to 0
 */
void initialize_io(void)
{
    if(pthread_mutex_lock(&inp_val_mutex)==0)
    {
        inp_val[0] = 0;
        inp_val[1] = 0;
    }
    if(pthread_mutex_unlock(&inp_val_mutex)!=0) { pi_reset_app(); }
    if(pthread_mutex_lock(&read_failure_mutex)==0)
    {
        read_failure = 0u;
    }
    if(pthread_mutex_unlock(&read_failure_mutex)!=0)    { pi_reset_app(); }
    
    integral_state = 0.0F;
}


/* void float32 get_direction(float32 signal_inp)
 *
 * get_direction returns 1.0F or 0.0F based on sign of direction
 */
static float32 get_direction(float32 signal_inp)
{
    unsigned char result;
    if (signal_inp >= 0.0F)
    {
        result = TRUE;
    }
    else
    {
        result = FALSE;
    }
    return (float32) result;
}

#include "pi_utils.h"

/* look_up_even provides a lookup table algorithm that works for evenly spaced values.
 *
 * Inputs to the function are...
 *     pY - pointer to the output value
 *     u - input value
 *     map - structure containing the static lookup table data...
 *         valueLo - minimum independent axis value
 *         uSpacing - increment size of evenly spaced independent axis
 *         iHi - number of increments available in pYData
 *     pYData - pointer to array of values that make up dependent axis of lookup table
 *
 */
float32 look_up_even(float32 u, map_data map, float32 *pYData, uint8 method)
{
	float32 pY;
    /* If input is below range of lookup table, output is minimum value of lookup table (pYData) */
    if (u <= map.valueLo)
    {
        pY = pYData[0];
    }
    else
    {
        /* Determine index of output into pYData based on input and uSpacing */
        float32 uAdjusted = u - map.valueLo;
        float32 iLeftFloat = (uAdjusted / map.uSpacing);
        uint32 iLeft = (uint32) iLeftFloat;
        
        /* If input is above range of lookup table, output is maximum value of lookup table (pYData) */
        if (iLeft >= map.iHi)
        {
            pY = pYData[map.iHi];
        }
        
        /* If input is in range of lookup table, output will interpolate between lookup values */
        else
        {
            
            float32 lambda;  /* fractional part of difference between input and nearest lower table value */
            
			float32 num = (uAdjusted - (((float32) iLeft) * map.uSpacing));
			lambda = (num/map.uSpacing);
		
			float32 yLeftCast;  /* table value that is just lower than input */
			float32 yRghtCast;  /* table value that is just higher than input */
			yLeftCast = pYData[iLeft];
			yRghtCast = pYData[((iLeft)+1U)];
			if ((method == 1U) && ((lambda > 1e-8F) || (lambda < -1e-8F)))
			{
				yLeftCast += (lambda * (yRghtCast - yLeftCast));
			}
			
			pY = yLeftCast;
        }
    }
	return pY;
}


/* void get_control_gains(float32* c_prop, float32* c_int)
 *
 * get_control_gains sets the values of Kp and Ki, the proportional
 * and integral gain constants used in the PI controller algorithm.
 *
 * The gain constants change based on how large the normalized error is
 */
void get_control_gains(float32* c_prop, float32* c_int)
{
    if ((normalized_error > 1.0F) || (normalized_error < -1.0F))  {
        /* if error is large */
        *c_prop = 0.5F;
        *c_int = 1.0F;
    } else {
        /* if error is small */
        *c_prop = 0.3F;
        *c_int = 2.0F;
    }
}


/* void saturate(float32* value, float lower, float upper)
 *
 * saturate limits the variable value between lower
 * and upper.
 */
void saturate(float32* value, float32 lower, float32 upper)
{
    if (*value < lower)
    {
        *value = lower;
    }
    if (*value > upper)
    {
        *value = upper;
    }
}

uint16 read_input_reg(void) {
    volatile uint16 x;
    return x;
}

void set_output_reg(int8 duty, int8 direction) {}


/* uint8 write_log_buffer(float32 error, float32 state, float32 output, uint8 pos)
 *
 * write_log_buffer collects the current normalized_error, integral_state, and
 * control_output in a global buffer_data structure at position pos and returns
 * the next valid position.
 */
uint8 write_log_buffer(float32 log_error, float32 state, float32 output, uint8 pos)
{
    /* Write values to buffer */
	uint8 pos_out;
    
    if(pthread_mutex_lock(&log_buffer_mutex)==0)
	{
	if (0 <pos && pos < 10) {
		log_buffer.error[pos] = log_error;
		log_buffer.state[pos] = state;
		log_buffer.output[pos] = output;
	}
	}
	if(pthread_mutex_unlock(&log_buffer_mutex)!=0)	{ pi_reset_app(); }
		
    /* Compute next position */
    pos_out = pos + 1;
    pos_out %= BUF_SIZE;
    return pos_out;
}

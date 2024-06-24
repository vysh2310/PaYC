#include "pi_task/pi_task.h"
#include "pi_alg/pi_alg.h"
#include "pi_task/pi_utils.h"

/* control_task implements a PI controller algorithm that ../
 *
 * - reads inputs from hardware on actual and desired position
 * - determines error between actual and desired position
 * - obtains controller gains
 * - calculates direction and duty cycle of PWM output using PI control algorithm
 * - sets PWM output to hardware
 *
 */
void control_task(bool buf_init)
{
    static uint8 pos;
    float32 Ki;
    float32 Kp;
    float32 control_output;
	uint8 method_reg[] = {0U,0U};
    
    /* Read inputs from hardware */
    inp_val[0] = read_input_reg();
    inp_val[1] = read_input_reg();
    
    /* Convert ADC values to their respective voltages provided read failure did not occur, otherwise assume 0 */
    if(pthread_mutex_lock(&read_failure_mutex)==0)
	{
		if (read_failure == 0u)
		{
			if(pthread_mutex_lock(&inp_val_mutex)==0)
			{
				inp_volt[0] = 0.0048828125F * (float32) inp_val[0];
				inp_volt[1] = 0.0048828125F * (float32) inp_val[1];
			}
			if(pthread_mutex_unlock(&inp_val_mutex)!=0) { pi_reset_app(); }
		}
		else {
			inp_volt[0] = 0.0F;
			inp_volt[1] = 0.0F;
		}
	}
    if(pthread_mutex_unlock(&read_failure_mutex)!=0) { pi_reset_app(); }  // demo: dup
    
    /* Determine error */
    process_inputs(method_reg);
    
    /* Determine integral and proportional controller gains */
    get_control_gains(&Kp,&Ki);
    
    /* PI control algorithm */
    control_output = pi_alg(Kp, Ki, 0);
    
    /* Set output pins on hardware */
    set_output_reg(duty_7bit,(int8)(direction));
    
    /* Buffer error, integral state, and output for logging*/
    if (buf_init != 0U)
    {
        pos = 0;
    }
    if(pthread_mutex_lock(&integral_state_mutex)==0)
	{
		pos = write_log_buffer(normalized_error, integral_state, control_output, pos+10);
	}
    if(pthread_mutex_unlock(&integral_state_mutex)!=0) { pi_reset_app(); }
}

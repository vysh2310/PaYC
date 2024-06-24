#include "pi_reset.h"

/* void state_reset(void)
 * state_reset sets the integral state to 0.0F. state_reset will be called as interrupt.
 */
void state_reset_ir(void)
{
    if(pthread_mutex_lock(&log_buffer_mutex)==0)
	{
		integral_state = 0.0F;
	}
    if(pthread_mutex_unlock(&log_buffer_mutex)!=0) { pi_reset_app(); }
}

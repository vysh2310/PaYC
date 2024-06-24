#include "pi_global.h"
#include "pi_alg/pi_alg.h"
#include "pi_reset/pi_reset.h"
#include "pi_log/pi_log.h"
#include "pi_task/pi_task.h"
#include <pthread.h>

#define BUFFERLEN 10
#define NUM_THREADS 3

/* Global variable definitions */
float32 angle_norm_vals[11] = {0.0F, 0.1F, 0.2F, 0.3F, 0.4F, 0.5F, 0.6F, 0.7F, 0.8F, 0.9F, 1.0F};
float32 pos_norm_vals[11] = {1.0F, 0.9F, 0.8F, 0.7F, 0.6F, 0.5F, 0.4F, 0.3F, 0.2F, 0.1F, 0.0F};

map_data angle_norm_map = { .valueLo = 0.0F, .iHi = 10, .uSpacing = 3.14159F/20.0F};
map_data pos_norm_map =  { .valueLo = 0.5F, .iHi = 10, .uSpacing = 0.4F};

pthread_mutex_t integral_state_mutex;
pthread_mutex_t inp_val_mutex;
pthread_mutex_t read_failure_mutex;
pthread_mutex_t log_buffer_mutex;

uint16 inp_val[NUM_IN];
float32 integral_state;
uint8 read_failure;
buffer_data log_buffer;

/* void* cyc_task1(void* args)
 *
 * entry point into periodic task which implements PI controller algorithm and processes I/O
 */
static void* cyc_task1(void* args);
static void* cyc_task1(void* args)  /* polyspace DEFECT:UNUSED_PARAMETER [Not a defect:Low] "Normal task signature, this task has no params" */
{
    int8 i;
    i++;
    while(1)
    {
        control_task(FALSE);
        for(i=1;i<BUFFERLEN;i++)
        {
            control_task(TRUE);
        }
    }
}


/* void* cyc_task2(void* args)
 *
 * entry point into periodic task which logs the output data buffer and stores it on a hard disk file
 */
static void* cyc_task2(void* args);
static void* cyc_task2(void* args)
{
    while(1)
    {
		log_task();
    }
}


/* void* intp_task(void* args)
 *
 * entry point into asynchronous interrupt task which resets integral state when triggered
 */
static void* intp_task(void* args);
static void* intp_task(void* args)
{
    while(1)
    {
        state_reset_ir();
    }
}

void pi_reset_app(void) {}


/* void main(void)
 *
 * entry point into program execution
 */
void main(void);
void main(void)
{
	pthread_t ph[NUM_THREADS];

	initialize_io();
	
	if(pthread_create(&ph[0],NULL,cyc_task1,NULL)!=0)	{ pi_reset_app(); }
	if(pthread_create(&ph[1],NULL,cyc_task2,NULL)!=0)	{ pi_reset_app(); }
	if(pthread_create(&ph[2],NULL,intp_task,NULL)!=0)	{ pi_reset_app(); }
}

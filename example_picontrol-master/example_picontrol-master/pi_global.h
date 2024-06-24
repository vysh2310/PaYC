#ifndef PI_GLOBAL_HEADER
#define PI_GLOBAL_HEADER

#include "pthread.h"
#include "pi_types.h"

#define TRUE (1U)
#define FALSE (0U)

/* Array size constants */
#define NUM_IN (2U)
#define NUM_NORM_VALS (11U)

/* defined in pi_alg.c */
extern float32 inp_volt[NUM_IN];
extern float32 normalized_error;

/* defined in pi_task.c */
extern float32 norm_factor;

/* defined in pi_log.c */
extern buffer_data log_buffer;

/* defined in pi_main.c */
extern uint8 read_failure;
extern uint16 inp_val[];
extern float32 integral_state;
extern map_data angle_norm_map;
extern map_data pos_norm_map;
extern float32 angle_norm_vals[NUM_NORM_VALS];
extern float32 pos_norm_vals[NUM_NORM_VALS];

/* mutexes for concurrent data access protection defined in pi_main.c */
extern pthread_mutex_t integral_state_mutex;
extern pthread_mutex_t inp_val_mutex;
extern pthread_mutex_t read_failure_mutex;
extern pthread_mutex_t log_buffer_mutex;

void pi_reset_app(void);
#endif

#ifndef ABC_IO
	#include <stdio.h>
	#include <time.h>
	#include <sys/time.h>
#else
	#include "abc_iotime.h"
#endif

#include "pi_log/pi_log.h"
#include "pi_task/pi_utils.h"

#define TIMESTRLEN 80

buffer_data log_buffer;

/* int32 write_log_file(void)
 * write_log_file appends the content of log_buffer to the file logfile.csv.
 * wrtie_log_file returns number of characters written
 * or negative if something went wrong.
 */
static int32 write_log_file(void);
static int32 write_log_file(void)
{
	int8 format_str[] = "%f\t%f\t%f\n";
	int8 time_str[TIMESTRLEN] = "INITIALIZATION\n";
	int8 mode[] = "a";
	#ifndef ABC_IO
		struct tm* tm_info = NULL;
		FILE* fp;
		time_t raw_time;
	#else
		struct abc_tm* abc_tm_info = NULL;
		ABC_FILE* abc_fp;
		abc_time_t abc_raw_time;
	#endif
	uint8 status;
	
	int32 write_success = -1;
	int32 bytes_written = 0;
	uint32 i;
	buffer_data *l = &log_buffer;
	
	#ifndef ABC_IO
		fp = fopen("logfile.csv",mode);

		if(fp!=NULL)
			{
			(void)time(&raw_time);
			tm_info = localtime(&raw_time);
			if(tm_info!=NULL)
			{
				if(strftime(time_str, TIMESTRLEN, "%x - %I:%M%p", tm_info) != 0U)
				{
					time_str[79] = (int8)('\0');
					write_success = fprintf(fp,time_str);
				}
			}
			if (write_success >= 0)
			{
				for (i=0 ; i<BUF_SIZE ; i++)
				{
					if(pthread_mutex_lock(&log_buffer_mutex)==0)
					{
						bytes_written = fprintf(fp,format_str,l->error[i],l->state[i],l->output[i]);
					}
				if(pthread_mutex_unlock(&log_buffer_mutex)!=0)	{ pi_reset_app(); }
					if (bytes_written < 0)
					{
						write_success = -1;
						break;
					}
					write_success =+ bytes_written;
				}
			}
			(void)fclose(fp);
		}
	#else
		abc_fp = abc_fopen("logfile.csv",mode);

		if(abc_fp!=NULL)
			{
			(void)abc_time(&abc_raw_time);
			abc_tm_info = abc_localtime(&abc_raw_time);
			if(abc_tm_info!=NULL)
			{
				if(abc_strftime(time_str, TIMESTRLEN, "%x - %I:%M%p", abc_tm_info) != 0)
				{
					time_str[79] = (int8)('\0');
					write_success = abc_fprintf(abc_fp,time_str);
				}
			}
			if (write_success >= 0)
			{
				for (i=0 ; i<BUF_SIZE ; i++)
				{
					if(pthread_mutex_lock(&log_buffer_mutex)==0)
					{
						bytes_written = abc_fprintf(abc_fp,format_str,l->error[i],l->state[i],l->output[i]);
					}
				if(pthread_mutex_unlock(&log_buffer_mutex)!=0)	{ pi_reset_app(); }
					if (bytes_written < 0)
					{
						write_success = -1;
						break;
					}
					write_success =+ bytes_written;
				}
			}
			(void)abc_fclose(abc_fp);
		}
	#endif
    return write_success;
}


/* void clear_log_buffer(void)
 * clear_log_buffer calls write_log_buffer to reset all log_buffer
 * fields' elements to 0.0F, after successful write_log_file.
 */
static void clear_log_buffer(void);
static void clear_log_buffer(void)
{
    uint8 i;
    float32 init = 0.0F;
    for (i=0 ; i<BUF_SIZE ; i++)
    {
        (void)write_log_buffer(init,init,init,i);
    }
}


/* void log_task(void)
 * log_task is called by scheduler to log pi_task's data from log_buffer to a file.
 * log_task calls write_log_file, and if successful clear_log_buffer
 */

void log_task(void)
{
	int32 write_success = -1;
    uint8 write_attempts = 0U;

    /* Try to write buffer to file, max. 5 attempts */
    while((write_success < 0) && (write_attempts < 5))
    {
        write_success = write_log_file();
        write_attempts++;
    }
    /* Clear buffer */
    clear_log_buffer();
}

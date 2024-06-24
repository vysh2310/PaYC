cmake_minimum_required(VERSION 2.8)

project(PID)
add_executable(runpid pi_app/pi_main.c 
	pi_app/pi_globals.c 
	pi_log/pi_log.c 
	pi_reset/pi_reset.c 
	pi_task/pi_task.c 
	pi_task/pi_utils.c
	pi_alg/pi_alg.c)

target_include_directories(runpid PUBLIC "${PROJECT_SOURCE_DIR}")
target_link_libraries(runpid m pthread)

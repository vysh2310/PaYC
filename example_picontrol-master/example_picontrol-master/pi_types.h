#ifndef PI_TYPES_HEADER
#define PI_TYPES_HEADER

// some change
#define BUF_SIZE (10U)

/* Typedefs for data types */
typedef unsigned char bool;
typedef unsigned char uint8;
typedef signed char int8;
typedef short int16;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef int int32;
typedef float float32;
typedef double float64;

/* Lookup table structure */
typedef struct {
    float32 valueLo;
    uint32 iHi;
    float32 uSpacing;
} map_data;

/* Buffer structure */
typedef struct {
    float32 error[BUF_SIZE];
    float32 state[BUF_SIZE];
    float32 output[BUF_SIZE];
} buffer_data;

#endif
